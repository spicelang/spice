; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [11 x i8] c"./test-dir\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"Done.\00", align 1
@str = private unnamed_addr constant [17 x i8] c"Creating dir ...\00", align 1
@str.1 = private unnamed_addr constant [17 x i8] c"Deleting dir ...\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @mkdir(ptr nocapture noundef readonly, i32 noundef) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @rmdir(ptr nocapture noundef readonly) local_unnamed_addr #0

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry.l7:
  %puts = tail call i32 @puts(ptr nonnull @str)
  %0 = tail call i32 @mkdir(ptr nonnull @0, i32 511)
  %puts2 = tail call i32 @puts(ptr nonnull @str.1)
  %1 = tail call i32 @rmdir(ptr nonnull @0)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull @1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
