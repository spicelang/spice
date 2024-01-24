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

while.head.L3:                                    ; preds = %if.exit.L5, %if.then.L10, %0
  %1 = load double, ptr %loopCounterOuter, align 8
  %2 = fcmp olt double %1, 1.000000e+01
  br i1 %2, label %while.body.L3, label %while.exit.L3

while.body.L3:                                    ; preds = %while.head.L3
  %3 = load double, ptr %loopCounterOuter, align 8
  %4 = fadd double %3, 1.500000e-01
  store double %4, ptr %loopCounterOuter, align 8
  %5 = load double, ptr %loopCounterOuter, align 8
  %6 = fcmp olt double %5, 4.000000e+00
  br i1 %6, label %if.then.L5, label %if.exit.L5

if.then.L5:                                       ; preds = %while.body.L3
  store i16 10, ptr %loopCounterInner, align 2
  br label %while.head.L7

while.head.L7:                                    ; preds = %if.exit.L10, %if.then.L5
  %7 = load i16, ptr %loopCounterInner, align 2
  %8 = sext i16 %7 to i32
  %9 = icmp sgt i32 %8, 0
  br i1 %9, label %while.body.L7, label %while.exit.L7

while.body.L7:                                    ; preds = %while.head.L7
  %10 = load double, ptr %loopCounterOuter, align 8
  %11 = load i16, ptr %loopCounterInner, align 2
  %12 = sext i16 %11 to i32
  %13 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %10, i32 %12)
  %14 = load i16, ptr %loopCounterInner, align 2
  %15 = sub nsw i16 %14, 1
  store i16 %15, ptr %loopCounterInner, align 2
  %16 = load i16, ptr %loopCounterInner, align 2
  %17 = sext i16 %16 to i32
  %18 = icmp eq i32 %17, 5
  br i1 %18, label %if.then.L10, label %if.exit.L10

if.then.L10:                                      ; preds = %while.body.L7
  br label %while.head.L3

if.exit.L10:                                      ; preds = %while.body.L7
  br label %while.head.L7

while.exit.L7:                                    ; preds = %while.head.L7
  br label %if.exit.L5

if.exit.L5:                                       ; preds = %while.exit.L7, %while.body.L3
  br label %while.head.L3

while.exit.L3:                                    ; preds = %while.head.L3
  %19 = load i32, ptr %result, align 4
  ret i32 %19
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
