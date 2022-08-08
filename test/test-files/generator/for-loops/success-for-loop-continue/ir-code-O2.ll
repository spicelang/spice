; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 1
@1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
for.l5.preheader.3:
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 0)
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 2)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 4)
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 6)
  br label %for.l5.3

for.l5.3:                                         ; preds = %for.l5.3, %for.l5.preheader.3
  %subCounter.010.3 = phi i32 [ %5, %for.l5.3 ], [ 100, %for.l5.preheader.3 ]
  %4 = tail call i32 (ptr, ...) @printf(ptr nonnull @1, i32 %subCounter.010.3)
  %5 = add nsw i32 %subCounter.010.3, -1
  %6 = add nsw i32 %subCounter.010.3, -12
  %7 = icmp ult i32 %6, -11
  br i1 %7, label %for.l5.3, label %for.l5.preheader.4

for.l5.preheader.4:                               ; preds = %for.l5.3
  %8 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 8)
  br label %for.l5.4

for.l5.4:                                         ; preds = %for.l5.4, %for.l5.preheader.4
  %subCounter.010.4 = phi i32 [ %10, %for.l5.4 ], [ 100, %for.l5.preheader.4 ]
  %9 = tail call i32 (ptr, ...) @printf(ptr nonnull @1, i32 %subCounter.010.4)
  %10 = add nsw i32 %subCounter.010.4, -1
  %11 = add nsw i32 %subCounter.010.4, -12
  %12 = icmp ult i32 %11, -11
  br i1 %12, label %for.l5.4, label %for.inc.l2.4

for.inc.l2.4:                                     ; preds = %for.l5.4
  %13 = tail call i32 (ptr, ...) @printf(ptr nonnull @2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
