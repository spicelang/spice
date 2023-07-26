; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [6 x i8] c"Pizza\00", align 1
@printf.str.0 = private unnamed_addr constant [32 x i8] c"Pointer address: %p, value: %s\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"Burger\00", align 1
@printf.str.1 = private unnamed_addr constant [20 x i8] c"Restored value: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [27 x i8] c"Restored value address: %p\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %food = alloca ptr, align 8
  %ptr = alloca ptr, align 8
  %restoredFood = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store ptr @anon.string.0, ptr %food, align 8
  store ptr %food, ptr %ptr, align 8
  %1 = load ptr, ptr %ptr, align 8
  %2 = load ptr, ptr %ptr, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %1, ptr %3)
  %5 = load ptr, ptr %ptr, align 8
  store ptr @anon.string.1, ptr %5, align 8
  %6 = load ptr, ptr %ptr, align 8
  %7 = load ptr, ptr %6, align 8
  store ptr %7, ptr %restoredFood, align 8
  %8 = load ptr, ptr %restoredFood, align 8
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %8)
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %restoredFood)
  %11 = load i32, ptr %result, align 4
  ret i32 %11
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
