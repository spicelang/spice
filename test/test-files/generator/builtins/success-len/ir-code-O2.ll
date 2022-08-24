; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@testIntArray = local_unnamed_addr global [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 0, i32 0, i32 0]
@0 = private unnamed_addr constant [18 x i8] c"Array length: %d\0A\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry.l1:
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 7)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
