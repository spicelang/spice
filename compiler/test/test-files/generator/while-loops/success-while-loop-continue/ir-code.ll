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
  %3 = alloca double, align 8
  %4 = alloca i32, align 4
  %5 = alloca i1, align 1
  %loopCounterInner = alloca i16, align 2
  %6 = alloca i16, align 2
  %7 = alloca i32, align 4
  %8 = alloca i1, align 1
  %9 = alloca i32, align 4
  %10 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store double 0.000000e+00, double* %0, align 8
  %11 = load double, double* %0, align 8
  store double %11, double* %loopCounterOuter, align 8
  br label %while.cond

while.cond:                                       ; preds = %if.end4, %if.then3, %entry
  store i32 10, i32* %1, align 4
  %12 = load double, double* %loopCounterOuter, align 8
  %13 = load i32, i32* %1, align 4
  %14 = sitofp i32 %13 to double
  %15 = fcmp olt double %12, %14
  store i1 %15, i1* %2, align 1
  %16 = load i1, i1* %2, align 1
  br i1 %16, label %while, label %while.end5

while:                                            ; preds = %while.cond
  store double 1.500000e-01, double* %3, align 8
  %17 = load double, double* %3, align 8
  %18 = load double, double* %loopCounterOuter, align 8
  %19 = fadd double %18, %17
  store double %19, double* %loopCounterOuter, align 8
  store i32 4, i32* %4, align 4
  %20 = load double, double* %loopCounterOuter, align 8
  %21 = load i32, i32* %4, align 4
  %22 = sitofp i32 %21 to double
  %23 = fcmp olt double %20, %22
  store i1 %23, i1* %5, align 1
  %24 = load i1, i1* %5, align 1
  br i1 %24, label %if.then, label %if.end4

if.then:                                          ; preds = %while
  store i16 10, i16* %6, align 2
  %25 = load i16, i16* %6, align 2
  store i16 %25, i16* %loopCounterInner, align 2
  br label %while.cond1

while.cond1:                                      ; preds = %if.end, %if.then
  store i32 0, i32* %7, align 4
  %26 = load i16, i16* %loopCounterInner, align 2
  %27 = load i32, i32* %7, align 4
  %28 = sext i16 %26 to i32
  %29 = icmp sgt i32 %28, %27
  store i1 %29, i1* %8, align 1
  %30 = load i1, i1* %8, align 1
  br i1 %30, label %while2, label %while.end

while2:                                           ; preds = %while.cond1
  %31 = load double, double* %loopCounterOuter, align 8
  %32 = load i16, i16* %loopCounterInner, align 2
  %33 = zext i16 %32 to i32
  %34 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), double %31, i32 %33)
  %35 = load i16, i16* %loopCounterInner, align 2
  %36 = sub i16 %35, 1
  store i16 %36, i16* %loopCounterInner, align 2
  store i32 5, i32* %9, align 4
  %37 = load i16, i16* %loopCounterInner, align 2
  %38 = load i32, i32* %9, align 4
  %39 = sext i16 %37 to i32
  %40 = icmp eq i32 %39, %38
  store i1 %40, i1* %10, align 1
  %41 = load i1, i1* %10, align 1
  br i1 %41, label %if.then3, label %if.end

if.then3:                                         ; preds = %while2
  br label %while.cond

if.end:                                           ; preds = %while2
  br label %while.cond1

while.end:                                        ; preds = %while.cond1
  br label %if.end4

if.end4:                                          ; preds = %while.end, %while
  br label %while.cond

while.end5:                                       ; preds = %while.cond
  %42 = load i32, i32* %result, align 4
  ret i32 %42
}
