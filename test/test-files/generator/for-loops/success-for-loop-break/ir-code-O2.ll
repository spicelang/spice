; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 1
@1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry.l1:
  br label %for.l2

for.l2:                                           ; preds = %for.l2, %entry.l1
  %counter.06 = phi i32 [ 0, %entry.l1 ], [ %2, %for.l2 ]
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 %counter.06)
  %1 = icmp ugt i32 %counter.06, 9000
  %2 = add nuw nsw i32 %counter.06, 2
  br i1 %1, label %for.l5, label %for.l2

for.l5:                                           ; preds = %for.l2, %for.l5
  %storemerge5 = phi i32 [ %5, %for.l5 ], [ 100, %for.l2 ]
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull @1, i32 %storemerge5)
  %4 = icmp eq i32 %storemerge5, 11
  %5 = add nsw i32 %storemerge5, -1
  br i1 %4, label %for.end.l2, label %for.l5

for.end.l2:                                       ; preds = %for.l5
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull @2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
