; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  br label %while

while:                                            ; preds = %entry, %while.cond.backedge
  %loopCounterOuter.016 = phi double [ 0.000000e+00, %entry ], [ %0, %while.cond.backedge ]
  %0 = fadd double %loopCounterOuter.016, 1.500000e-01
  %1 = fcmp olt double %0, 4.000000e+00
  br i1 %1, label %while.cond.backedge.loopexit, label %while.cond.backedge

while.cond.backedge.loopexit:                     ; preds = %while
  %2 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([22 x i8], [22 x i8]* @0, i64 0, i64 0), double %0, i32 10)
  %3 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([22 x i8], [22 x i8]* @0, i64 0, i64 0), double %0, i32 9)
  %4 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([22 x i8], [22 x i8]* @0, i64 0, i64 0), double %0, i32 8)
  %5 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([22 x i8], [22 x i8]* @0, i64 0, i64 0), double %0, i32 7)
  %6 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([22 x i8], [22 x i8]* @0, i64 0, i64 0), double %0, i32 6)
  br label %while.cond.backedge

while.cond.backedge:                              ; preds = %while.cond.backedge.loopexit, %while
  %7 = fcmp olt double %0, 1.000000e+01
  br i1 %7, label %while, label %while.end5

while.end5:                                       ; preds = %while.cond.backedge
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
