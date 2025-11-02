; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  br label %dowhile.body.L3

dowhile.body.L3:                                  ; preds = %dowhile.foot.L3, %0
  %loopCounterOuter.0 = phi double [ 0.000000e+00, %0 ], [ %1, %dowhile.foot.L3 ]
  %1 = fadd double %loopCounterOuter.0, 1.500000e-01
  %2 = fcmp olt double %1, 4.000000e+00
  br i1 %2, label %dowhile.body.L7.preheader, label %dowhile.foot.L3

dowhile.body.L7.preheader:                        ; preds = %dowhile.body.L3
  %3 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double noundef %1, i32 noundef 10)
  %4 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double noundef %1, i32 noundef 9)
  %5 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double noundef %1, i32 noundef 8)
  %6 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double noundef %1, i32 noundef 7)
  %7 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, double noundef %1, i32 noundef 6)
  br label %dowhile.foot.L3

dowhile.foot.L3:                                  ; preds = %dowhile.body.L7.preheader, %dowhile.body.L3
  %8 = fcmp olt double %1, 1.000000e+01
  br i1 %8, label %dowhile.body.L3, label %dowhile.exit.L3

dowhile.exit.L3:                                  ; preds = %dowhile.foot.L3
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
