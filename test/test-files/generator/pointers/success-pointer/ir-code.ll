; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [6 x i8] c"Pizza\00", align 1
@1 = private unnamed_addr constant [32 x i8] c"Pointer address: %p, value: %s\0A\00", align 1
@2 = private unnamed_addr constant [7 x i8] c"Burger\00", align 1
@3 = private unnamed_addr constant [20 x i8] c"Restored value: %s\0A\00", align 1
@4 = private unnamed_addr constant [27 x i8] c"Restored value address: %p\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %food = alloca ptr, align 8
  %0 = alloca ptr, align 8
  %ptr = alloca ptr, align 8
  %restoredFood = alloca ptr, align 8
  %1 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store ptr @0, ptr %food, align 8
  store ptr %food, ptr %0, align 8
  %2 = load ptr, ptr %0, align 8
  store ptr %2, ptr %ptr, align 8
  %3 = load ptr, ptr %ptr, align 8
  %4 = load ptr, ptr %ptr, align 8
  %5 = load ptr, ptr %4, align 8
  store ptr %5, ptr %4, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = call i32 (ptr, ...) @printf(ptr @1, ptr %3, ptr %6)
  %8 = load ptr, ptr %ptr, align 8
  %9 = load ptr, ptr %8, align 8
  store ptr %9, ptr %8, align 8
  store ptr @2, ptr %8, align 8
  %10 = load ptr, ptr %ptr, align 8
  %11 = load ptr, ptr %10, align 8
  store ptr %11, ptr %10, align 8
  %12 = load ptr, ptr %10, align 8
  store ptr %12, ptr %restoredFood, align 8
  %13 = load ptr, ptr %restoredFood, align 8
  %14 = call i32 (ptr, ...) @printf(ptr @3, ptr %13)
  store ptr %restoredFood, ptr %1, align 8
  %15 = load ptr, ptr %1, align 8
  %16 = call i32 (ptr, ...) @printf(ptr @4, ptr %15)
  %17 = load i32, ptr %result, align 4
  ret i32 %17
}

declare i32 @printf(ptr, ...)
