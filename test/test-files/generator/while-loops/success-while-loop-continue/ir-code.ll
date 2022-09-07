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
  %4 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  store double 0.000000e+00, ptr %loopCounterOuter, align 8
  br label %while.cond.l3

while.cond.l3:                                    ; preds = %if.end.l5, %if.then.l10, %entry.l1
  %5 = load double, ptr %loopCounterOuter, align 8
  %6 = fcmp olt double %5, 1.000000e+01
  store i1 %6, ptr %0, align 1
  %7 = load i1, ptr %0, align 1
  br i1 %7, label %while.l3, label %while.end.l3

while.l3:                                         ; preds = %while.cond.l3
  %8 = load double, ptr %loopCounterOuter, align 8
  %9 = fadd double %8, 1.500000e-01
  store double %9, ptr %loopCounterOuter, align 8
  %10 = load double, ptr %loopCounterOuter, align 8
  %11 = fcmp olt double %10, 4.000000e+00
  store i1 %11, ptr %1, align 1
  %12 = load i1, ptr %1, align 1
  br i1 %12, label %if.then.l5, label %if.end.l5

if.then.l5:                                       ; preds = %while.l3
  store i16 10, ptr %loopCounterInner, align 2
  br label %while.cond.l7

while.cond.l7:                                    ; preds = %if.end.l10, %if.then.l5
  %13 = load i16, ptr %loopCounterInner, align 2
  %14 = sext i16 %13 to i32
  %15 = icmp sgt i32 %14, 0
  store i1 %15, ptr %2, align 1
  %16 = load i1, ptr %2, align 1
  br i1 %16, label %while.l7, label %while.end.l7

while.l7:                                         ; preds = %while.cond.l7
  %17 = load double, ptr %loopCounterOuter, align 8
  %18 = load i16, ptr %loopCounterInner, align 2
  %19 = sext i16 %18 to i32
  %20 = call i32 (ptr, ...) @printf(ptr @0, double %17, i32 %19)
  %21 = load i16, ptr %loopCounterInner, align 2
  %22 = sub i16 %21, 1
  store i16 %22, ptr %loopCounterInner, align 2
  store i16 %21, ptr %3, align 2
  %23 = load i16, ptr %loopCounterInner, align 2
  %24 = sext i16 %23 to i32
  %25 = icmp eq i32 %24, 5
  store i1 %25, ptr %4, align 1
  %26 = load i1, ptr %4, align 1
  br i1 %26, label %if.then.l10, label %if.end.l10

if.then.l10:                                      ; preds = %while.l7
  br label %while.cond.l3

if.end.l10:                                       ; preds = %while.l7
  br label %while.cond.l7

while.end.l7:                                     ; preds = %while.cond.l7
  br label %if.end.l5

if.end.l5:                                        ; preds = %while.end.l7, %while.l3
  br label %while.cond.l3

while.end.l3:                                     ; preds = %while.cond.l3
  %27 = load i32, ptr %result, align 4
  ret i32 %27
}

declare i32 @printf(ptr, ...)
