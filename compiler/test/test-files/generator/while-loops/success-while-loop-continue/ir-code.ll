; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

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
  %9 = alloca i16, align 2
  %10 = alloca i32, align 4
  %11 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store double 0.000000e+00, double* %0, align 8
  %12 = load double, double* %0, align 8
  store double %12, double* %loopCounterOuter, align 8
  br label %while.cond

while.cond:                                       ; preds = %if.end4, %if.then3, %entry
  store i32 10, i32* %1, align 4
  %13 = load double, double* %loopCounterOuter, align 8
  %14 = load i32, i32* %1, align 4
  %15 = sitofp i32 %14 to double
  %16 = fcmp olt double %13, %15
  store i1 %16, i1* %2, align 1
  %17 = load i1, i1* %2, align 1
  br i1 %17, label %while, label %while.end5

while:                                            ; preds = %while.cond
  store double 1.500000e-01, double* %3, align 8
  %18 = load double, double* %3, align 8
  %19 = load double, double* %loopCounterOuter, align 8
  %20 = fadd double %19, %18
  store double %20, double* %loopCounterOuter, align 8
  store i32 4, i32* %4, align 4
  %21 = load double, double* %loopCounterOuter, align 8
  %22 = load i32, i32* %4, align 4
  %23 = sitofp i32 %22 to double
  %24 = fcmp olt double %21, %23
  store i1 %24, i1* %5, align 1
  %25 = load i1, i1* %5, align 1
  br i1 %25, label %if.then, label %if.end4

if.then:                                          ; preds = %while
  store i16 10, i16* %6, align 2
  %26 = load i16, i16* %6, align 2
  store i16 %26, i16* %loopCounterInner, align 2
  br label %while.cond1

while.cond1:                                      ; preds = %if.end, %if.then
  store i32 0, i32* %7, align 4
  %27 = load i16, i16* %loopCounterInner, align 2
  %28 = load i32, i32* %7, align 4
  %29 = sext i16 %27 to i32
  %30 = icmp sgt i32 %29, %28
  store i1 %30, i1* %8, align 1
  %31 = load i1, i1* %8, align 1
  br i1 %31, label %while2, label %while.end

while2:                                           ; preds = %while.cond1
  %32 = load double, double* %loopCounterOuter, align 8
  %33 = load i16, i16* %loopCounterInner, align 2
  %34 = zext i16 %33 to i32
  %35 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), double %32, i32 %34)
  %36 = load i16, i16* %loopCounterInner, align 2
  %37 = sub i16 %36, 1
  store i16 %37, i16* %loopCounterInner, align 2
  store i16 %36, i16* %9, align 2
  store i32 5, i32* %10, align 4
  %38 = load i16, i16* %loopCounterInner, align 2
  %39 = load i32, i32* %10, align 4
  %40 = sext i16 %38 to i32
  %41 = icmp eq i32 %40, %39
  store i1 %41, i1* %11, align 1
  %42 = load i1, i1* %11, align 1
  br i1 %42, label %if.then3, label %if.end

if.then3:                                         ; preds = %while2
  br label %while.cond

if.end:                                           ; preds = %while2
  br label %while.cond1

while.end:                                        ; preds = %while.cond1
  br label %if.end4

if.end4:                                          ; preds = %while.end, %while
  br label %while.cond

while.end5:                                       ; preds = %while.cond
  %43 = load i32, i32* %result, align 4
  ret i32 %43
}

declare i32 @printf(i8*, ...)
