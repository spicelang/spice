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
  %8 = alloca i16, align 2
  %9 = alloca double, align 8
  store i32 0, i32* %result, align 4
  store double 0.000000e+00, double* %0, align 8
  %10 = load double, double* %0, align 8
  store double %10, double* %loopCounterOuter, align 8
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  store i32 10, i32* %1, align 4
  %11 = load double, double* %loopCounterOuter, align 8
  %12 = load i32, i32* %1, align 4
  %13 = sitofp i32 %12 to double
  %14 = fcmp olt double %11, %13
  store i1 %14, i1* %2, align 1
  %15 = load i1, i1* %2, align 1
  br i1 %15, label %while, label %while.end3

while:                                            ; preds = %while.cond
  store i32 4, i32* %3, align 4
  %16 = load double, double* %loopCounterOuter, align 8
  %17 = load i32, i32* %3, align 4
  %18 = sitofp i32 %17 to double
  %19 = fcmp olt double %16, %18
  store i1 %19, i1* %4, align 1
  %20 = load i1, i1* %4, align 1
  br i1 %20, label %if.then, label %if.end

if.then:                                          ; preds = %while
  store i16 10, i16* %5, align 2
  %21 = load i16, i16* %5, align 2
  store i16 %21, i16* %loopCounterInner, align 2
  br label %while.cond1

while.cond1:                                      ; preds = %if.then
  store i32 0, i32* %6, align 4
  %22 = load i16, i16* %loopCounterInner, align 2
  %23 = load i32, i32* %6, align 4
  %24 = sext i16 %22 to i32
  %25 = icmp sgt i32 %24, %23
  store i1 %25, i1* %7, align 1
  %26 = load i1, i1* %7, align 1
  br i1 %26, label %while2, label %while.end

while2:                                           ; preds = %while.cond1
  %27 = load double, double* %loopCounterOuter, align 8
  %28 = load i16, i16* %loopCounterInner, align 2
  %29 = zext i16 %28 to i32
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), double %27, i32 %29)
  %31 = load i16, i16* %loopCounterInner, align 2
  %32 = sub i16 %31, 1
  store i16 %32, i16* %loopCounterInner, align 2
  store i16 %31, i16* %8, align 2
  br label %while.end3

while.end:                                        ; preds = %while.cond1
  br label %if.end

if.end:                                           ; preds = %while.end, %while
  store double 1.500000e-01, double* %9, align 8
  %33 = load double, double* %9, align 8
  %34 = load double, double* %loopCounterOuter, align 8
  %35 = fadd double %34, %33
  store double %35, double* %loopCounterOuter, align 8
  br label %while.cond

while.end3:                                       ; preds = %while2, %while.cond
  %36 = load i32, i32* %result, align 4
  ret i32 %36
}
