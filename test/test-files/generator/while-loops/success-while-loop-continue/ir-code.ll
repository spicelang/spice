; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %loopCounterOuter = alloca double, align 8
  store double 0.000000e+00, ptr %loopCounterOuter, align 8
  br label %while.cond.l3

while.cond.l3:                                    ; preds = %if.end.l5, %if.then.l10, %entry.l1
  %0 = load double, ptr %loopCounterOuter, align 8
  %1 = fcmp olt double %0, 1.000000e+01
  %2 = alloca i1, align 1
  store i1 %1, ptr %2, align 1
  %3 = load i1, ptr %2, align 1
  br i1 %3, label %while.l3, label %while.end.l3

while.l3:                                         ; preds = %while.cond.l3
  %4 = load double, ptr %loopCounterOuter, align 8
  %5 = fadd double %4, 1.500000e-01
  store double %5, ptr %loopCounterOuter, align 8
  %6 = load double, ptr %loopCounterOuter, align 8
  %7 = fcmp olt double %6, 4.000000e+00
  %8 = alloca i1, align 1
  store i1 %7, ptr %8, align 1
  %9 = load i1, ptr %8, align 1
  br i1 %9, label %if.then.l5, label %if.end.l5

if.then.l5:                                       ; preds = %while.l3
  %loopCounterInner = alloca i16, align 2
  store i16 10, ptr %loopCounterInner, align 2
  br label %while.cond.l7

while.cond.l7:                                    ; preds = %if.end.l10, %if.then.l5
  %10 = load i16, ptr %loopCounterInner, align 2
  %11 = sext i16 %10 to i32
  %12 = icmp sgt i32 %11, 0
  %13 = alloca i1, align 1
  store i1 %12, ptr %13, align 1
  %14 = load i1, ptr %13, align 1
  br i1 %14, label %while.l7, label %while.end.l7

while.l7:                                         ; preds = %while.cond.l7
  %15 = load double, ptr %loopCounterOuter, align 8
  %16 = load i16, ptr %loopCounterInner, align 2
  %17 = sext i16 %16 to i32
  %18 = call i32 (ptr, ...) @printf(ptr @0, double %15, i32 %17)
  %19 = load i16, ptr %loopCounterInner, align 2
  %20 = sub i16 %19, 1
  store i16 %20, ptr %loopCounterInner, align 2
  %21 = alloca i16, align 2
  store i16 %19, ptr %21, align 2
  %22 = load i16, ptr %loopCounterInner, align 2
  %23 = sext i16 %22 to i32
  %24 = icmp eq i32 %23, 5
  %25 = alloca i1, align 1
  store i1 %24, ptr %25, align 1
  %26 = load i1, ptr %25, align 1
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
