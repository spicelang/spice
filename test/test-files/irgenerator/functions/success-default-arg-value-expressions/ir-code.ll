; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Test: %f\0A\00", align 1

define private double @_Z6getArgv() {
  %result = alloca double, align 8
  ret double 4.300000e+00
}

define private double @_Z4testv() {
  %result = alloca double, align 8
  %arg = alloca double, align 8
  %1 = call double @_Z6getArgv()
  %2 = fadd double %1, 1.200000e+00
  store double %2, ptr %arg, align 8
  %3 = load double, ptr %arg, align 8
  ret double %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call double @_Z4testv()
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
