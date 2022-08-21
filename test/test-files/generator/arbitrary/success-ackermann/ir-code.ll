; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [36 x i8] c"Ackermann of base m=%d and n=%d: %d\00", align 1

define internal i32 @_f__void__ack__int_int(i32 %0, i32 %1) {
entry.l1:
  %m = alloca i32, align 4
  store i32 %0, ptr %m, align 4
  %n = alloca i32, align 4
  store i32 %1, ptr %n, align 4
  %result = alloca i32, align 4
  %2 = load i32, ptr %m, align 4
  %3 = icmp eq i32 %2, 0
  %4 = alloca i1, align 1
  store i1 %3, ptr %4, align 1
  %5 = load i1, ptr %4, align 1
  br i1 %5, label %if.then.l2, label %if.end.l2

if.then.l2:                                       ; preds = %entry.l1
  %6 = load i32, ptr %n, align 4
  %7 = add i32 %6, 1
  %8 = alloca i32, align 4
  store i32 %7, ptr %8, align 4
  %9 = load i32, ptr %8, align 4
  ret i32 %9

if.end.l2:                                        ; preds = %entry.l1
  %10 = load i32, ptr %n, align 4
  %11 = icmp eq i32 %10, 0
  %12 = alloca i1, align 1
  store i1 %11, ptr %12, align 1
  %13 = load i1, ptr %12, align 1
  br i1 %13, label %if.then.l3, label %if.end.l3

if.then.l3:                                       ; preds = %if.end.l2
  %14 = load i32, ptr %m, align 4
  %15 = sub i32 %14, 1
  %16 = alloca i32, align 4
  store i32 %15, ptr %16, align 4
  %17 = load i32, ptr %16, align 4
  %18 = alloca i32, align 4
  store i32 1, ptr %18, align 4
  %19 = load i32, ptr %18, align 4
  %20 = call i32 @_f__void__ack__int_int(i32 %17, i32 %19)
  %21 = alloca i32, align 4
  store i32 %20, ptr %21, align 4
  %22 = load i32, ptr %21, align 4
  ret i32 %22

if.end.l3:                                        ; preds = %if.end.l2
  %23 = load i32, ptr %m, align 4
  %24 = sub i32 %23, 1
  %25 = alloca i32, align 4
  store i32 %24, ptr %25, align 4
  %26 = load i32, ptr %25, align 4
  %27 = load i32, ptr %m, align 4
  %28 = load i32, ptr %n, align 4
  %29 = sub i32 %28, 1
  %30 = alloca i32, align 4
  store i32 %29, ptr %30, align 4
  %31 = load i32, ptr %30, align 4
  %32 = call i32 @_f__void__ack__int_int(i32 %27, i32 %31)
  %33 = alloca i32, align 4
  store i32 %32, ptr %33, align 4
  %34 = load i32, ptr %33, align 4
  %35 = call i32 @_f__void__ack__int_int(i32 %26, i32 %34)
  %36 = alloca i32, align 4
  store i32 %35, ptr %36, align 4
  %37 = load i32, ptr %36, align 4
  ret i32 %37
}

define i32 @main() {
entry.l7:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %baseM = alloca i32, align 4
  store i32 3, ptr %baseM, align 4
  %baseN = alloca i32, align 4
  store i32 12, ptr %baseN, align 4
  %0 = load i32, ptr %baseM, align 4
  %1 = load i32, ptr %baseN, align 4
  %2 = load i32, ptr %baseM, align 4
  %3 = load i32, ptr %baseN, align 4
  %4 = call i32 @_f__void__ack__int_int(i32 %2, i32 %3)
  %5 = alloca i32, align 4
  store i32 %4, ptr %5, align 4
  %6 = load i32, ptr %5, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @0, i32 %0, i32 %1, i32 %6)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

declare i32 @printf(ptr, ...)
