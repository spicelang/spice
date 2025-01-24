; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %loopCounterOuter = alloca double, align 8
  %loopCounterInner = alloca i16, align 2
  store i32 0, ptr %result, align 4
  store double 0.000000e+00, ptr %loopCounterOuter, align 8
  br label %dowhile.body.L3

dowhile.body.L3:                                  ; preds = %dowhile.foot.L3, %0
  %1 = load double, ptr %loopCounterOuter, align 8
  %2 = fcmp olt double %1, 4.000000e+00
  br i1 %2, label %if.then.L4, label %if.exit.L4

if.then.L4:                                       ; preds = %dowhile.body.L3
  store i16 10, ptr %loopCounterInner, align 2
  br label %dowhile.body.L6

dowhile.body.L6:                                  ; preds = %dowhile.foot.L6, %if.then.L4
  %3 = load double, ptr %loopCounterOuter, align 8
  %4 = load i16, ptr %loopCounterInner, align 2
  %5 = sext i16 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %3, i32 %5)
  %7 = load i16, ptr %loopCounterInner, align 2
  %8 = sub nsw i16 %7, 1
  store i16 %8, ptr %loopCounterInner, align 2
  br label %dowhile.exit.L3

dowhile.foot.L6:                                  ; No predecessors!
  %9 = load i16, ptr %loopCounterInner, align 2
  %10 = sext i16 %9 to i32
  %11 = icmp sgt i32 %10, 0
  br i1 %11, label %dowhile.body.L6, label %dowhile.exit.L6

dowhile.exit.L6:                                  ; preds = %dowhile.foot.L6
  br label %if.exit.L4

if.exit.L4:                                       ; preds = %dowhile.exit.L6, %dowhile.body.L3
  %12 = load double, ptr %loopCounterOuter, align 8
  %13 = fadd double %12, 1.500000e-01
  store double %13, ptr %loopCounterOuter, align 8
  br label %dowhile.foot.L3

dowhile.foot.L3:                                  ; preds = %if.exit.L4
  %14 = load double, ptr %loopCounterOuter, align 8
  %15 = fcmp olt double %14, 1.000000e+01
  br i1 %15, label %dowhile.body.L3, label %dowhile.exit.L3

dowhile.exit.L3:                                  ; preds = %dowhile.foot.L3, %dowhile.body.L6
  %16 = load i32, ptr %result, align 4
  ret i32 %16
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
