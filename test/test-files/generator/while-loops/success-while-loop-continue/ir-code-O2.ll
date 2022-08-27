; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry.l1:
  br label %while.l3

while.l3:                                         ; preds = %entry.l1, %while.cond.l3.backedge
  %loopCounterOuter.04 = phi double [ 0.000000e+00, %entry.l1 ], [ %0, %while.cond.l3.backedge ]
  %0 = fadd double %loopCounterOuter.04, 1.500000e-01
  %1 = fcmp olt double %0, 4.000000e+00
  br i1 %1, label %while.cond.l3.backedge.loopexit, label %while.cond.l3.backedge

while.cond.l3.backedge.loopexit:                  ; preds = %while.l3
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, double %0, i32 10)
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, double %0, i32 9)
  %4 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, double %0, i32 8)
  %5 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, double %0, i32 7)
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, double %0, i32 6)
  br label %while.cond.l3.backedge

while.cond.l3.backedge:                           ; preds = %while.cond.l3.backedge.loopexit, %while.l3
  %7 = fcmp olt double %0, 1.000000e+01
  br i1 %7, label %while.l3, label %while.end.l3

while.end.l3:                                     ; preds = %while.cond.l3.backedge
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
