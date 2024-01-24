; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %loopCounterOuter = alloca double, align 8
  %loopCounterInner = alloca i16, align 2
  store i32 0, ptr %result, align 4
  store double 0.000000e+00, ptr %loopCounterOuter, align 8
  br label %while.head.L3

while.head.L3:                                    ; preds = %if.exit.L4, %0
  %1 = load double, ptr %loopCounterOuter, align 8
  %2 = fcmp olt double %1, 1.000000e+01
  br i1 %2, label %while.body.L3, label %while.exit.L3

while.body.L3:                                    ; preds = %while.head.L3
  %3 = load double, ptr %loopCounterOuter, align 8
  %4 = fcmp olt double %3, 4.000000e+00
  br i1 %4, label %if.then.L4, label %if.exit.L4

if.then.L4:                                       ; preds = %while.body.L3
  store i16 10, ptr %loopCounterInner, align 2
  br label %while.head.L6

while.head.L6:                                    ; preds = %if.then.L4
  %5 = load i16, ptr %loopCounterInner, align 2
  %6 = sext i16 %5 to i32
  %7 = icmp sgt i32 %6, 0
  br i1 %7, label %while.body.L6, label %while.exit.L6

while.body.L6:                                    ; preds = %while.head.L6
  %8 = load double, ptr %loopCounterOuter, align 8
  %9 = load i16, ptr %loopCounterInner, align 2
  %10 = sext i16 %9 to i32
  %11 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %8, i32 %10)
  %12 = load i16, ptr %loopCounterInner, align 2
  %13 = sub nsw i16 %12, 1
  store i16 %13, ptr %loopCounterInner, align 2
  br label %while.exit.L3

while.exit.L6:                                    ; preds = %while.head.L6
  br label %if.exit.L4

if.exit.L4:                                       ; preds = %while.exit.L6, %while.body.L3
  %14 = load double, ptr %loopCounterOuter, align 8
  %15 = fadd double %14, 1.500000e-01
  store double %15, ptr %loopCounterOuter, align 8
  br label %while.head.L3

while.exit.L3:                                    ; preds = %while.body.L6, %while.head.L3
  %16 = load i32, ptr %result, align 4
  ret i32 %16
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
