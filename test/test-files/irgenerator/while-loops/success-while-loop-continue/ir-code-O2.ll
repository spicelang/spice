; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  br label %while.body.L3

while.body.L3:                                    ; preds = %0, %while.head.L3.backedge
  %loopCounterOuter.08 = phi double [ 0.000000e+00, %0 ], [ %1, %while.head.L3.backedge ]
  %1 = fadd double %loopCounterOuter.08, 1.500000e-01
  %2 = fcmp olt double %1, 4.000000e+00
  br i1 %2, label %while.head.L3.backedge.loopexit, label %while.head.L3.backedge

while.head.L3.backedge.loopexit:                  ; preds = %while.body.L3
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, double %1, i32 10)
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, double %1, i32 9)
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, double %1, i32 8)
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, double %1, i32 7)
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, double %1, i32 6)
  br label %while.head.L3.backedge

while.head.L3.backedge:                           ; preds = %while.head.L3.backedge.loopexit, %while.body.L3
  %8 = fcmp olt double %1, 1.000000e+01
  br i1 %8, label %while.body.L3, label %while.exit.L3

while.exit.L3:                                    ; preds = %while.head.L3.backedge
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
