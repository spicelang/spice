; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__ShoppingItem__string_double_string = type { ptr, double, ptr }
%__ShoppingCart__string_ShoppingItemarray = type { ptr, [3 x %__ShoppingItem__string_double_string] }

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.0 = private unnamed_addr constant [10 x i8] c"Spaghetti\00", align 1
@anon.string.1 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@anon.struct.0 = private unnamed_addr constant %__ShoppingItem__string_double_string { ptr @anon.string.0, double 1.000000e+02, ptr @anon.string.1 }
@anon.string.2 = private unnamed_addr constant [5 x i8] c"Rice\00", align 1
@anon.string.3 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@anon.struct.1 = private unnamed_addr constant %__ShoppingItem__string_double_string { ptr @anon.string.2, double 1.255000e+02, ptr @anon.string.3 }
@anon.string.4 = private unnamed_addr constant [9 x i8] c"Doughnut\00", align 1
@anon.string.5 = private unnamed_addr constant [4 x i8] c"pcs\00", align 1
@anon.struct.2 = private unnamed_addr constant %__ShoppingItem__string_double_string { ptr @anon.string.4, double 6.000000e+00, ptr @anon.string.5 }
@anon.string.6 = private unnamed_addr constant [14 x i8] c"Shopping Cart\00", align 1
@anon.string.7 = private unnamed_addr constant [10 x i8] c"Spaghetti\00", align 1
@anon.string.8 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@anon.struct.3 = private unnamed_addr constant %__ShoppingItem__string_double_string { ptr @anon.string.7, double 1.000000e+02, ptr @anon.string.8 }
@anon.string.9 = private unnamed_addr constant [5 x i8] c"Rice\00", align 1
@anon.string.10 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@anon.struct.4 = private unnamed_addr constant %__ShoppingItem__string_double_string { ptr @anon.string.9, double 1.255000e+02, ptr @anon.string.10 }
@anon.string.11 = private unnamed_addr constant [9 x i8] c"Doughnut\00", align 1
@anon.string.12 = private unnamed_addr constant [4 x i8] c"pcs\00", align 1
@anon.struct.5 = private unnamed_addr constant %__ShoppingItem__string_double_string { ptr @anon.string.11, double 6.000000e+00, ptr @anon.string.12 }
@anon.array.0 = private unnamed_addr constant [3 x %__ShoppingItem__string_double_string] [%__ShoppingItem__string_double_string { ptr @anon.string.7, double 1.000000e+02, ptr @anon.string.8 }, %__ShoppingItem__string_double_string { ptr @anon.string.9, double 1.255000e+02, ptr @anon.string.10 }, %__ShoppingItem__string_double_string { ptr @anon.string.11, double 6.000000e+00, ptr @anon.string.12 }]
@anon.string.13 = private unnamed_addr constant [13 x i8] c"Another Cart\00", align 1
@printf.str.0 = private unnamed_addr constant [26 x i8] c"Shopping cart item 1: %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [30 x i8] c"Another cart item 2 unit: %s\0A\00", align 1

define private %__ShoppingCart__string_ShoppingItemarray @_f__void__ShoppingCart__newShoppingCart() {
  %result = alloca %__ShoppingCart__string_ShoppingItemarray, align 8
  %items = alloca [3 x %__ShoppingItem__string_double_string], align 8
  %1 = alloca %__ShoppingCart__string_ShoppingItemarray, align 8
  store [3 x %__ShoppingItem__string_double_string] [%__ShoppingItem__string_double_string { ptr @0, double 0.000000e+00, ptr @1 }, %__ShoppingItem__string_double_string { ptr @0, double 0.000000e+00, ptr @1 }, %__ShoppingItem__string_double_string { ptr @0, double 0.000000e+00, ptr @1 }], ptr %items, align 8
  %2 = getelementptr inbounds [3 x %__ShoppingItem__string_double_string], ptr %items, i32 0, i32 0
  call void @llvm.memcpy.p0.p0.i64(ptr %2, ptr @anon.struct.0, i64 24, i1 false)
  %3 = getelementptr inbounds [3 x %__ShoppingItem__string_double_string], ptr %items, i32 0, i32 1
  call void @llvm.memcpy.p0.p0.i64(ptr %3, ptr @anon.struct.1, i64 24, i1 false)
  %4 = getelementptr inbounds [3 x %__ShoppingItem__string_double_string], ptr %items, i32 0, i32 2
  call void @llvm.memcpy.p0.p0.i64(ptr %4, ptr @anon.struct.2, i64 24, i1 false)
  %5 = getelementptr inbounds %__ShoppingCart__string_ShoppingItemarray, ptr %1, i32 0, i32 0
  store ptr @anon.string.6, ptr %5, align 8
  %6 = load [3 x %__ShoppingItem__string_double_string], ptr %items, align 8
  %7 = getelementptr inbounds %__ShoppingCart__string_ShoppingItemarray, ptr %1, i32 0, i32 1
  store [3 x %__ShoppingItem__string_double_string] %6, ptr %7, align 8
  %8 = load %__ShoppingCart__string_ShoppingItemarray, ptr %1, align 8
  ret %__ShoppingCart__string_ShoppingItemarray %8
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #0

define private %__ShoppingCart__string_ShoppingItemarray @_f__void__ShoppingCart__anotherShoppingCart() {
  %result = alloca %__ShoppingCart__string_ShoppingItemarray, align 8
  %1 = alloca [3 x %__ShoppingItem__string_double_string], align 8
  %items = alloca [3 x %__ShoppingItem__string_double_string], align 8
  %2 = alloca %__ShoppingCart__string_ShoppingItemarray, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.array.0, i64 72, i1 false)
  store [3 x %__ShoppingItem__string_double_string] [%__ShoppingItem__string_double_string { ptr @anon.string.7, double 1.000000e+02, ptr @anon.string.8 }, %__ShoppingItem__string_double_string { ptr @anon.string.9, double 1.255000e+02, ptr @anon.string.10 }, %__ShoppingItem__string_double_string { ptr @anon.string.11, double 6.000000e+00, ptr @anon.string.12 }], ptr %items, align 8
  %3 = getelementptr inbounds %__ShoppingCart__string_ShoppingItemarray, ptr %2, i32 0, i32 0
  store ptr @anon.string.13, ptr %3, align 8
  %4 = load [3 x %__ShoppingItem__string_double_string], ptr %items, align 8
  %5 = getelementptr inbounds %__ShoppingCart__string_ShoppingItemarray, ptr %2, i32 0, i32 1
  store [3 x %__ShoppingItem__string_double_string] %4, ptr %5, align 8
  %6 = load %__ShoppingCart__string_ShoppingItemarray, ptr %2, align 8
  ret %__ShoppingCart__string_ShoppingItemarray %6
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %shoppingCart = alloca %__ShoppingCart__string_ShoppingItemarray, align 8
  %1 = alloca %__ShoppingCart__string_ShoppingItemarray, align 8
  store i32 0, ptr %result, align 4
  %2 = call %__ShoppingCart__string_ShoppingItemarray @_f__void__ShoppingCart__newShoppingCart()
  store %__ShoppingCart__string_ShoppingItemarray %2, ptr %shoppingCart, align 8
  %items = getelementptr inbounds %__ShoppingCart__string_ShoppingItemarray, ptr %shoppingCart, i32 0, i32 1
  %3 = getelementptr inbounds [3 x %__ShoppingItem__string_double_string], ptr %items, i32 0, i32 1
  %name = getelementptr inbounds %__ShoppingItem__string_double_string, ptr %3, i32 0, i32 0
  %4 = load ptr, ptr %name, align 8
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %4)
  %6 = call %__ShoppingCart__string_ShoppingItemarray @_f__void__ShoppingCart__anotherShoppingCart()
  store %__ShoppingCart__string_ShoppingItemarray %6, ptr %1, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %shoppingCart, ptr %1, i64 80, i1 false)
  %items1 = getelementptr inbounds %__ShoppingCart__string_ShoppingItemarray, ptr %shoppingCart, i32 0, i32 1
  %7 = getelementptr inbounds [3 x %__ShoppingItem__string_double_string], ptr %items1, i32 0, i32 2
  %unit = getelementptr inbounds %__ShoppingItem__string_double_string, ptr %7, i32 0, i32 2
  %8 = load ptr, ptr %unit, align 8
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #1 = { noinline nounwind optnone uwtable }
