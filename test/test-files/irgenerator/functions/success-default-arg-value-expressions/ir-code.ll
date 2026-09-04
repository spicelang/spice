; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Test: %f\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef double @_Z6getArgv() #0 {
  ret double 4.300000e+00
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef double @_Z4testv() #0 {
  %arg = alloca double, align 8
  %1 = call noundef double @_Z6getArgv()
  %2 = fadd double %1, 1.200000e+00
  store double %2, ptr %arg, align 8
  %3 = load double, ptr %arg, align 8
  ret double %3
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %1 = call noundef double @_Z4testv()
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double noundef %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
