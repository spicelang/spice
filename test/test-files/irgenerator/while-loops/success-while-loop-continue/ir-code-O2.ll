; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  br label %while.body.L3

while.body.L3:                                    ; preds = %0, %while.head.L3.backedge
  %loopCounterOuter.08 = phi double [ 0.000000e+00, %0 ], [ %1, %while.head.L3.backedge ]
  %1 = fadd double %loopCounterOuter.08, 1.500000e-01
  %2 = fcmp olt double %1, 4.000000e+00
  br i1 %2, label %while.head.L3.backedge.loopexit, label %while.head.L3.backedge

while.head.L3.backedge.loopexit:                  ; preds = %while.body.L3
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double %1, i32 10)
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double %1, i32 9)
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double %1, i32 8)
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double %1, i32 7)
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double %1, i32 6)
  br label %while.head.L3.backedge

while.head.L3.backedge:                           ; preds = %while.head.L3.backedge.loopexit, %while.body.L3
  %8 = fcmp olt double %1, 1.000000e+01
  br i1 %8, label %while.body.L3, label %while.exit.L3

while.exit.L3:                                    ; preds = %while.head.L3.backedge
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
