; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [36 x i8] c"Ackermann of base m=%d and n=%d: %d\00", align 1

define internal i32 @_f__void__ack__int_int(i32 %0, i32 %1) {
entry.l1:
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  %result = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store i32 %0, i32* %m, align 4
  store i32 %1, i32* %n, align 4
  %12 = load i32, i32* %m, align 4
  %13 = icmp eq i32 %12, 0
  store i1 %13, i1* %2, align 1
  %14 = load i1, i1* %2, align 1
  br i1 %14, label %if.then.l2, label %if.end.l2

if.then.l2:                                       ; preds = %entry.l1
  %15 = load i32, i32* %n, align 4
  %16 = add i32 %15, 1
  store i32 %16, i32* %3, align 4
  %17 = load i32, i32* %3, align 4
  ret i32 %17

if.end.l2:                                        ; preds = %entry.l1
  %18 = load i32, i32* %n, align 4
  %19 = icmp eq i32 %18, 0
  store i1 %19, i1* %4, align 1
  %20 = load i1, i1* %4, align 1
  br i1 %20, label %if.then.l3, label %if.end.l3

if.then.l3:                                       ; preds = %if.end.l2
  %21 = load i32, i32* %m, align 4
  %22 = sub i32 %21, 1
  store i32 %22, i32* %5, align 4
  %23 = load i32, i32* %5, align 4
  store i32 1, i32* %6, align 4
  %24 = load i32, i32* %6, align 4
  %25 = call i32 @_f__void__ack__int_int(i32 %23, i32 %24)
  store i32 %25, i32* %7, align 4
  %26 = load i32, i32* %7, align 4
  ret i32 %26

if.end.l3:                                        ; preds = %if.end.l2
  %27 = load i32, i32* %m, align 4
  %28 = sub i32 %27, 1
  store i32 %28, i32* %8, align 4
  %29 = load i32, i32* %8, align 4
  %30 = load i32, i32* %m, align 4
  %31 = load i32, i32* %n, align 4
  %32 = sub i32 %31, 1
  store i32 %32, i32* %9, align 4
  %33 = load i32, i32* %9, align 4
  %34 = call i32 @_f__void__ack__int_int(i32 %30, i32 %33)
  store i32 %34, i32* %10, align 4
  %35 = load i32, i32* %10, align 4
  %36 = call i32 @_f__void__ack__int_int(i32 %29, i32 %35)
  store i32 %36, i32* %11, align 4
  %37 = load i32, i32* %11, align 4
  ret i32 %37
}

define i32 @main() {
entry.l7:
  %result = alloca i32, align 4
  %baseM = alloca i32, align 4
  %baseN = alloca i32, align 4
  %0 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 3, i32* %baseM, align 4
  store i32 12, i32* %baseN, align 4
  %1 = load i32, i32* %baseM, align 4
  %2 = load i32, i32* %baseN, align 4
  %3 = load i32, i32* %baseM, align 4
  %4 = load i32, i32* %baseN, align 4
  %5 = call i32 @_f__void__ack__int_int(i32 %3, i32 %4)
  store i32 %5, i32* %0, align 4
  %6 = load i32, i32* %0, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @0, i32 0, i32 0), i32 %1, i32 %2, i32 %6)
  %8 = load i32, i32* %result, align 4
  ret i32 %8
}

declare i32 @printf(i8*, ...)
