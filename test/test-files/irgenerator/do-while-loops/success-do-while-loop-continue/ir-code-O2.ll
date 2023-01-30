; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  br label %dowhile.body.L3

dowhile.body.L3:                                  ; preds = %dowhile.foot.L3, %0
  %loopCounterOuter.0 = phi double [ 0.000000e+00, %0 ], [ %1, %dowhile.foot.L3 ]
  %1 = fadd double %loopCounterOuter.0, 1.500000e-01
  %2 = fcmp olt double %1, 4.000000e+00
  br i1 %2, label %dowhile.body.L7, label %dowhile.foot.L3

dowhile.body.L7:                                  ; preds = %dowhile.body.L3, %dowhile.body.L7
  %loopCounterInner.0 = phi i16 [ %5, %dowhile.body.L7 ], [ 10, %dowhile.body.L3 ]
  %3 = zext i16 %loopCounterInner.0 to i32
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double %1, i32 %3)
  %5 = add nsw i16 %loopCounterInner.0, -1
  switch i16 %loopCounterInner.0, label %dowhile.body.L7 [
    i16 6, label %dowhile.foot.L3
    i16 1, label %dowhile.foot.L3
  ]

dowhile.foot.L3:                                  ; preds = %dowhile.body.L7, %dowhile.body.L7, %dowhile.body.L3
  %6 = fcmp olt double %1, 1.000000e+01
  br i1 %6, label %dowhile.body.L3, label %dowhile.exit.L3

dowhile.exit.L3:                                  ; preds = %dowhile.foot.L3
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
