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
  %1 = alloca i1, align 1
  %2 = alloca i1, align 1
  %loopCounterInner = alloca i16, align 2
  %3 = alloca i16, align 2
  %4 = alloca i1, align 1
  %5 = alloca i16, align 2
  %6 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store double 0.000000e+00, double* %0, align 8
  br label %while.cond

while.cond:                                       ; preds = %if.end4, %if.then3, %entry
  %7 = load double, double* %0, align 8
  %8 = fcmp olt double %7, 1.000000e+01
  store i1 %8, i1* %1, align 1
  %9 = load i1, i1* %1, align 1
  br i1 %9, label %while, label %while.end5

while:                                            ; preds = %while.cond
  %10 = load double, double* %0, align 8
  %11 = fadd double %10, 1.500000e-01
  store double %11, double* %0, align 8
  %12 = load double, double* %0, align 8
  %13 = fcmp olt double %12, 4.000000e+00
  store i1 %13, i1* %2, align 1
  %14 = load i1, i1* %2, align 1
  br i1 %14, label %if.then, label %if.end4

if.then:                                          ; preds = %while
  store i16 10, i16* %3, align 2
  br label %while.cond1

while.cond1:                                      ; preds = %if.end, %if.then
  %15 = load i16, i16* %3, align 2
  %16 = sext i16 %15 to i32
  %17 = icmp sgt i32 %16, 0
  store i1 %17, i1* %4, align 1
  %18 = load i1, i1* %4, align 1
  br i1 %18, label %while2, label %while.end

while2:                                           ; preds = %while.cond1
  %19 = load double, double* %0, align 8
  %20 = load i16, i16* %3, align 2
  %21 = sext i16 %20 to i32
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), double %19, i32 %21)
  %23 = load i16, i16* %3, align 2
  %24 = sub i16 %23, 1
  store i16 %24, i16* %3, align 2
  store i16 %23, i16* %5, align 2
  %25 = load i16, i16* %3, align 2
  %26 = sext i16 %25 to i32
  %27 = icmp eq i32 %26, 5
  store i1 %27, i1* %6, align 1
  %28 = load i1, i1* %6, align 1
  br i1 %28, label %if.then3, label %if.end

if.then3:                                         ; preds = %while2
  br label %while.cond

if.end:                                           ; preds = %while2
  br label %while.cond1

while.end:                                        ; preds = %while.cond1
  br label %if.end4

if.end4:                                          ; preds = %while.end, %while
  br label %while.cond

while.end5:                                       ; preds = %while.cond
  %29 = load i32, i32* %result, align 4
  ret i32 %29
}

declare i32 @printf(i8*, ...)
