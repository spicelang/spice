; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__ShoppingCart__string_ShoppingItemarray = type { ptr, [3 x %_s__ShoppingItem__string_double_string] }
%_s__ShoppingItem__string_double_string = type { ptr, double, ptr }

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@2 = private unnamed_addr constant [10 x i8] c"Spaghetti\00", align 1
@3 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@4 = private unnamed_addr constant [5 x i8] c"Rice\00", align 1
@5 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@6 = private unnamed_addr constant [9 x i8] c"Doughnut\00", align 1
@7 = private unnamed_addr constant [4 x i8] c"pcs\00", align 1
@8 = private unnamed_addr constant [14 x i8] c"Shopping Cart\00", align 1
@9 = private unnamed_addr constant [10 x i8] c"Spaghetti\00", align 1
@10 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@11 = private unnamed_addr constant [5 x i8] c"Rice\00", align 1
@12 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@13 = private unnamed_addr constant [9 x i8] c"Doughnut\00", align 1
@14 = private unnamed_addr constant [4 x i8] c"pcs\00", align 1
@15 = private unnamed_addr constant [13 x i8] c"Another Cart\00", align 1
@16 = private unnamed_addr constant [26 x i8] c"Shopping cart item 1: %s\0A\00", align 1
@17 = private unnamed_addr constant [30 x i8] c"Another cart item 2 unit: %s\0A\00", align 1

define internal %_s__ShoppingCart__string_ShoppingItemarray @_f__void__newShoppingCart() {
entry.l14:
  %result = alloca %_s__ShoppingCart__string_ShoppingItemarray, align 8
  %0 = alloca %_s__ShoppingItem__string_double_string, align 8
  %items = alloca [3 x %_s__ShoppingItem__string_double_string], align 8
  %1 = alloca %_s__ShoppingItem__string_double_string, align 8
  %2 = alloca %_s__ShoppingItem__string_double_string, align 8
  %3 = alloca %_s__ShoppingItem__string_double_string, align 8
  %4 = alloca %_s__ShoppingCart__string_ShoppingItemarray, align 8
  %5 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %0, i32 0, i32 0
  store ptr @0, ptr %5, align 8
  %6 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %0, i32 0, i32 1
  store double 0.000000e+00, ptr %6, align 8
  %7 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %0, i32 0, i32 2
  store ptr @1, ptr %7, align 8
  store [3 x %_s__ShoppingItem__string_double_string] [%_s__ShoppingItem__string_double_string { ptr @0, double 0.000000e+00, ptr @1 }, %_s__ShoppingItem__string_double_string { ptr @0, double 0.000000e+00, ptr @1 }, %_s__ShoppingItem__string_double_string { ptr @0, double 0.000000e+00, ptr @1 }], ptr %items, align 8
  %8 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %1, i32 0, i32 0
  store ptr @2, ptr %8, align 8
  %9 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %1, i32 0, i32 1
  store double 1.000000e+02, ptr %9, align 8
  %10 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %1, i32 0, i32 2
  store ptr @3, ptr %10, align 8
  %11 = load %_s__ShoppingItem__string_double_string, ptr %1, align 8
  %12 = getelementptr inbounds [3 x %_s__ShoppingItem__string_double_string], ptr %items, i32 0, i32 0
  store %_s__ShoppingItem__string_double_string %11, ptr %12, align 8
  %13 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %2, i32 0, i32 0
  store ptr @4, ptr %13, align 8
  %14 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %2, i32 0, i32 1
  store double 1.255000e+02, ptr %14, align 8
  %15 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %2, i32 0, i32 2
  store ptr @5, ptr %15, align 8
  %16 = load %_s__ShoppingItem__string_double_string, ptr %2, align 8
  %17 = getelementptr inbounds [3 x %_s__ShoppingItem__string_double_string], ptr %items, i32 0, i32 1
  store %_s__ShoppingItem__string_double_string %16, ptr %17, align 8
  %18 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %3, i32 0, i32 0
  store ptr @6, ptr %18, align 8
  %19 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %3, i32 0, i32 1
  store double 6.000000e+00, ptr %19, align 8
  %20 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %3, i32 0, i32 2
  store ptr @7, ptr %20, align 8
  %21 = load %_s__ShoppingItem__string_double_string, ptr %3, align 8
  %22 = getelementptr inbounds [3 x %_s__ShoppingItem__string_double_string], ptr %items, i32 0, i32 2
  store %_s__ShoppingItem__string_double_string %21, ptr %22, align 8
  %23 = getelementptr inbounds %_s__ShoppingCart__string_ShoppingItemarray, ptr %4, i32 0, i32 0
  store ptr @8, ptr %23, align 8
  %24 = load [3 x %_s__ShoppingItem__string_double_string], ptr %items, align 8
  %25 = getelementptr inbounds %_s__ShoppingCart__string_ShoppingItemarray, ptr %4, i32 0, i32 1
  store [3 x %_s__ShoppingItem__string_double_string] %24, ptr %25, align 8
  %26 = load %_s__ShoppingCart__string_ShoppingItemarray, ptr %4, align 8
  ret %_s__ShoppingCart__string_ShoppingItemarray %26
}

define internal %_s__ShoppingCart__string_ShoppingItemarray @_f__void__anotherShoppingCart() {
entry.l22:
  %result = alloca %_s__ShoppingCart__string_ShoppingItemarray, align 8
  %0 = alloca %_s__ShoppingItem__string_double_string, align 8
  %1 = alloca %_s__ShoppingItem__string_double_string, align 8
  %2 = alloca %_s__ShoppingItem__string_double_string, align 8
  %3 = alloca [3 x %_s__ShoppingItem__string_double_string], align 8
  %4 = alloca %_s__ShoppingCart__string_ShoppingItemarray, align 8
  %5 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %0, i32 0, i32 0
  store ptr @9, ptr %5, align 8
  %6 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %0, i32 0, i32 1
  store double 1.000000e+02, ptr %6, align 8
  %7 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %0, i32 0, i32 2
  store ptr @10, ptr %7, align 8
  %8 = load %_s__ShoppingItem__string_double_string, ptr %0, align 8
  %9 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %1, i32 0, i32 0
  store ptr @11, ptr %9, align 8
  %10 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %1, i32 0, i32 1
  store double 1.255000e+02, ptr %10, align 8
  %11 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %1, i32 0, i32 2
  store ptr @12, ptr %11, align 8
  %12 = load %_s__ShoppingItem__string_double_string, ptr %1, align 8
  %13 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %2, i32 0, i32 0
  store ptr @13, ptr %13, align 8
  %14 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %2, i32 0, i32 1
  store double 6.000000e+00, ptr %14, align 8
  %15 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %2, i32 0, i32 2
  store ptr @14, ptr %15, align 8
  %16 = load %_s__ShoppingItem__string_double_string, ptr %2, align 8
  %17 = getelementptr inbounds [3 x %_s__ShoppingItem__string_double_string], ptr %3, i32 0, i32 0
  store %_s__ShoppingItem__string_double_string %8, ptr %17, align 8
  %18 = getelementptr inbounds [3 x %_s__ShoppingItem__string_double_string], ptr %3, i32 0, i32 1
  store %_s__ShoppingItem__string_double_string %12, ptr %18, align 8
  %19 = getelementptr inbounds [3 x %_s__ShoppingItem__string_double_string], ptr %3, i32 0, i32 2
  store %_s__ShoppingItem__string_double_string %16, ptr %19, align 8
  %20 = getelementptr inbounds %_s__ShoppingCart__string_ShoppingItemarray, ptr %4, i32 0, i32 0
  store ptr @15, ptr %20, align 8
  %21 = load [3 x %_s__ShoppingItem__string_double_string], ptr %3, align 8
  %22 = getelementptr inbounds %_s__ShoppingCart__string_ShoppingItemarray, ptr %4, i32 0, i32 1
  store [3 x %_s__ShoppingItem__string_double_string] %21, ptr %22, align 8
  %23 = load %_s__ShoppingCart__string_ShoppingItemarray, ptr %4, align 8
  ret %_s__ShoppingCart__string_ShoppingItemarray %23
}

define i32 @main() {
entry.l31:
  %result = alloca i32, align 4
  %0 = alloca %_s__ShoppingCart__string_ShoppingItemarray, align 8
  %1 = alloca %_s__ShoppingCart__string_ShoppingItemarray, align 8
  store i32 0, ptr %result, align 4
  %2 = call %_s__ShoppingCart__string_ShoppingItemarray @_f__void__newShoppingCart()
  store %_s__ShoppingCart__string_ShoppingItemarray %2, ptr %0, align 8
  %3 = getelementptr inbounds %_s__ShoppingCart__string_ShoppingItemarray, ptr %0, i32 0, i32 1
  %4 = getelementptr inbounds [3 x %_s__ShoppingItem__string_double_string], ptr %3, i32 0, i32 1
  %5 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %4, i32 0, i32 0
  %6 = load ptr, ptr %5, align 8
  %7 = call i32 (ptr, ...) @printf(ptr @16, ptr %6)
  %8 = call %_s__ShoppingCart__string_ShoppingItemarray @_f__void__anotherShoppingCart()
  store %_s__ShoppingCart__string_ShoppingItemarray %8, ptr %1, align 8
  %9 = load %_s__ShoppingCart__string_ShoppingItemarray, ptr %1, align 8
  store %_s__ShoppingCart__string_ShoppingItemarray %9, ptr %0, align 8
  %10 = getelementptr inbounds %_s__ShoppingCart__string_ShoppingItemarray, ptr %0, i32 0, i32 1
  %11 = getelementptr inbounds [3 x %_s__ShoppingItem__string_double_string], ptr %10, i32 0, i32 2
  %12 = getelementptr inbounds %_s__ShoppingItem__string_double_string, ptr %11, i32 0, i32 2
  %13 = load ptr, ptr %12, align 8
  %14 = call i32 (ptr, ...) @printf(ptr @17, ptr %13)
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

declare i32 @printf(ptr, ...)
