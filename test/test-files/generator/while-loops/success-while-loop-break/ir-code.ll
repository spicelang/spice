; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %loopCounterOuter = alloca double, align 8
  %0 = alloca i1, align 1
  %1 = alloca i1, align 1
  %loopCounterInner = alloca i16, align 2
  %2 = alloca i1, align 1
  %3 = alloca i16, align 2
  store i32 0, i32* %result, align 4
  store double 0.000000e+00, double* %loopCounterOuter, align 8
  br label %while.cond.l3

while.cond.l3:                                    ; preds = %if.end.l4, %entry.l1
  %4 = load double, double* %loopCounterOuter, align 8
  %5 = fcmp olt double %4, 1.000000e+01
  store i1 %5, i1* %0, align 1
  %6 = load i1, i1* %0, align 1
  br i1 %6, label %while.l3, label %while.end.l3

while.l3:                                         ; preds = %while.cond.l3
  %7 = load double, double* %loopCounterOuter, align 8
  %8 = fcmp olt double %7, 4.000000e+00
  store i1 %8, i1* %1, align 1
  %9 = load i1, i1* %1, align 1
  br i1 %9, label %if.then.l4, label %if.end.l4

if.then.l4:                                       ; preds = %while.l3
  store i16 10, i16* %loopCounterInner, align 2
  br label %while.cond.l6

while.cond.l6:                                    ; preds = %if.then.l4
  %10 = load i16, i16* %loopCounterInner, align 2
  %11 = sext i16 %10 to i32
  %12 = icmp sgt i32 %11, 0
  store i1 %12, i1* %2, align 1
  %13 = load i1, i1* %2, align 1
  br i1 %13, label %while.l6, label %while.end.l6

while.l6:                                         ; preds = %while.cond.l6
  %14 = load double, double* %loopCounterOuter, align 8
  %15 = load i16, i16* %loopCounterInner, align 2
  %16 = sext i16 %15 to i32
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), double %14, i32 %16)
  %18 = load i16, i16* %loopCounterInner, align 2
  %19 = sub i16 %18, 1
  store i16 %19, i16* %loopCounterInner, align 2
  store i16 %18, i16* %3, align 2
  br label %while.end.l3

while.end.l6:                                     ; preds = %while.cond.l6
  br label %if.end.l4

if.end.l4:                                        ; preds = %while.end.l6, %while.l3
  %20 = load double, double* %loopCounterOuter, align 8
  %21 = fadd double %20, 1.500000e-01
  store double %21, double* %loopCounterOuter, align 8
  br label %while.cond.l3

while.end.l3:                                     ; preds = %while.l6, %while.cond.l3
  %22 = load i32, i32* %result, align 4
  ret i32 %22
}

declare i32 @printf(i8*, ...)
