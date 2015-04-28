(* libguestfs
 * Copyright (C) 2009-2014 Red Hat Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *)

(* Please read generator/README first. *)

open Unix
open Printf

open Pr
open Actions
open Structs
open Api_versions
open Types

open C
open Xdr
open Daemon
open Bindtests
open Errnostring
open Customize

let perror msg = function
  | Unix_error (err, _, _) ->
      eprintf "%s: %s\n" msg (error_message err)
  | exn ->
      eprintf "%s: %s\n" msg (Printexc.to_string exn)

(* Main program. *)
let () =
  let lock_fd =
    try openfile "BUGS" [O_RDWR] 0
    with
    | Unix_error (ENOENT, _, _) ->
        eprintf "\
You are probably running this from the wrong directory.
Run it from the top source directory using the command
  make -C generator stamp-generator
";
        exit 1
    | exn ->
        perror "open: BUGS" exn;
        exit 1 in

  (* Acquire a lock so parallel builds won't try to run the generator
   * twice at the same time.  Subsequent builds will wait for the first
   * one to finish.  Note the lock is released implicitly when the
   * program exits.
   *)
  (try lockf lock_fd F_LOCK 1
   with exn ->
     perror "lock: BUGS" exn;
     exit 1);

  let test = "" in
  (* Read the API versions file. *)
  load_api_versions (test ^ "src/api-support/added");

  output_to "src/guestfs_protocol.x" generate_xdr;
  output_to "src/guestfs.h" generate_guestfs_h;
  output_to "src/guestfs-internal-actions.h" generate_internal_actions_h;
  output_to "src/guestfs-internal-frontend-cleanups.h"
    generate_internal_frontend_cleanups_h;
  output_to "src/bindtests.c" generate_bindtests;
  output_to "src/guestfs-structs.pod" generate_structs_pod;
  output_to "src/guestfs-actions.pod" generate_actions_pod;
  output_to "src/guestfs-availability.pod" generate_availability_pod;
  output_to "src/errnostring-gperf.gperf" generate_errnostring_gperf;
  output_to "src/errnostring.c" generate_errnostring_c;
  output_to "src/errnostring.h" generate_errnostring_h;
  output_to "src/event-string.c" generate_event_string_c;
  output_to "src/MAX_PROC_NR" generate_max_proc_nr;
  output_to "src/libguestfs.syms" generate_linker_script;

  output_to "src/structs-compare.c" generate_client_structs_compare;
  output_to "src/structs-copy.c" generate_client_structs_copy;
  output_to "src/structs-free.c" generate_client_structs_free;
  output_to "src/structs-cleanup.c" generate_client_structs_cleanup;
  output_to "src/actions-variants.c" generate_client_actions_variants;

  for i = 0 to nr_actions_files-1 do
    let filename = sprintf "src/actions-%d.c" i in
    output_to filename (generate_client_actions i)
  done;

  output_to "daemon/guestfs_protocol.x" generate_xdr;
  output_to "daemon/actions.h" generate_daemon_actions_h;
  output_to "daemon/stubs.c" generate_daemon_actions;
  output_to "daemon/names.c" generate_daemon_names;
  output_to "daemon/optgroups.c" generate_daemon_optgroups_c;
  output_to "daemon/optgroups.h" generate_daemon_optgroups_h;

  (* Generate the list of files generated -- last. *)
  printf "generated %d lines of code\n" (get_lines_generated ());
  let files = List.sort compare (get_files_generated ()) in
  output_to "generator/files-generated.txt"
    (fun () -> List.iter (pr "%s\n") files)
