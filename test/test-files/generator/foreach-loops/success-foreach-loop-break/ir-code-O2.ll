; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@anonymous.0 = local_unnamed_addr constant [2 x i64] [i64 1, i64 2]
@l = local_unnamed_addr constant [2 x i64] [i64 1, i64 2]
@1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
foreach.end.l4:
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 1)
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull @1, i64 1)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull @2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
