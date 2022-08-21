; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [6 x i8] c"Pizza\00", align 1
@1 = private unnamed_addr constant [31 x i8] c"Pointer address: %p, value: %s\00", align 1
@2 = private unnamed_addr constant [7 x i8] c"Burger\00", align 1
@3 = private unnamed_addr constant [19 x i8] c"Restored value: %s\00", align 1
@4 = private unnamed_addr constant [28 x i8] c"Restored value address: %p\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %food = alloca ptr, align 8
  store ptr @0, ptr %food, align 8
  %0 = alloca ptr, align 8
  store ptr %food, ptr %0, align 8
  %1 = load ptr, ptr %0, align 8
  %2 = load ptr, ptr %0, align 8
  %3 = load ptr, ptr %2, align 8
  store ptr %3, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  %5 = call i32 (ptr, ...) @printf(ptr @1, ptr %1, ptr %4)
  %6 = load ptr, ptr %0, align 8
  %7 = load ptr, ptr %6, align 8
  store ptr %7, ptr %6, align 8
  store ptr @2, ptr %6, align 8
  %8 = load ptr, ptr %0, align 8
  %9 = load ptr, ptr %8, align 8
  store ptr %9, ptr %8, align 8
  %10 = load ptr, ptr %8, align 8
  %11 = call i32 (ptr, ...) @printf(ptr @3, ptr %10)
  %12 = alloca ptr, align 8
  store ptr %8, ptr %12, align 8
  %13 = load ptr, ptr %12, align 8
  %14 = call i32 (ptr, ...) @printf(ptr @4, ptr %13)
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

declare i32 @printf(ptr, ...)
