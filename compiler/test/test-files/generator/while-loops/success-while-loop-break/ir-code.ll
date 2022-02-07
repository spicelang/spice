; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %loopCounterOuter = alloca double, align 8
  %0 = alloca double, align 8
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  %loopCounterInner = alloca i16, align 2
  %5 = alloca i16, align 2
  %6 = alloca i32, align 4
  %7 = alloca i1, align 1
  %8 = alloca double, align 8
  store i32 0, i32* %result, align 4
  store double 0.000000e+00, double* %0, align 8
  %9 = load double, double* %0, align 8
  store double %9, double* %loopCounterOuter, align 8
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  store i32 10, i32* %1, align 4
  %10 = load double, double* %loopCounterOuter, align 8
  %11 = load i32, i32* %1, align 4
  %12 = sitofp i32 %11 to double
  %13 = fcmp olt double %10, %12
  store i1 %13, i1* %2, align 1
  %14 = load i1, i1* %2, align 1
  br i1 %14, label %while, label %while.end3

while:                                            ; preds = %while.cond
  store i32 4, i32* %3, align 4
  %15 = load double, double* %loopCounterOuter, align 8
  %16 = load i32, i32* %3, align 4
  %17 = sitofp i32 %16 to double
  %18 = fcmp olt double %15, %17
  store i1 %18, i1* %4, align 1
  %19 = load i1, i1* %4, align 1
  br i1 %19, label %if.then, label %if.end

if.then:                                          ; preds = %while
  store i16 10, i16* %5, align 2
  %20 = load i16, i16* %5, align 2
  store i16 %20, i16* %loopCounterInner, align 2
  br label %while.cond1

while.cond1:                                      ; preds = %if.then
  store i32 0, i32* %6, align 4
  %21 = load i16, i16* %loopCounterInner, align 2
  %22 = load i32, i32* %6, align 4
  %23 = sext i16 %21 to i32
  %24 = icmp sgt i32 %23, %22
  store i1 %24, i1* %7, align 1
  %25 = load i1, i1* %7, align 1
  br i1 %25, label %while2, label %while.end

while2:                                           ; preds = %while.cond1
  %26 = load double, double* %loopCounterOuter, align 8
  %27 = load i16, i16* %loopCounterInner, align 2
  %28 = zext i16 %27 to i32
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), double %26, i32 %28)
  %30 = load i16, i16* %loopCounterInner, align 2
  %31 = sub i16 %30, 1
  store i16 %31, i16* %loopCounterInner, align 2
  br label %while.end3

while.end:                                        ; preds = %while.cond1
  br label %if.end

if.end:                                           ; preds = %while.end, %while
  store double 1.500000e-01, double* %8, align 8
  %32 = load double, double* %8, align 8
  %33 = load double, double* %loopCounterOuter, align 8
  %34 = fadd double %33, %32
  store double %34, double* %loopCounterOuter, align 8
  br label %while.cond

while.end3:                                       ; preds = %while2, %while.cond
  %35 = load i32, i32* %result, align 4
  ret i32 %35
}
