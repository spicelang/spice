; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.ShoppingItem = type { ptr, double, ptr }
%struct.ShoppingCart = type { ptr, [3 x %struct.ShoppingItem] }

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.0 = private unnamed_addr constant [10 x i8] c"Spaghetti\00", align 1
@anon.string.1 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@anon.string.2 = private unnamed_addr constant [5 x i8] c"Rice\00", align 1
@anon.string.3 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@anon.string.4 = private unnamed_addr constant [9 x i8] c"Doughnut\00", align 1
@anon.string.5 = private unnamed_addr constant [4 x i8] c"pcs\00", align 1
@anon.string.6 = private unnamed_addr constant [14 x i8] c"Shopping Cart\00", align 1
@anon.string.7 = private unnamed_addr constant [10 x i8] c"Spaghetti\00", align 1
@anon.string.8 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@anon.string.9 = private unnamed_addr constant [5 x i8] c"Rice\00", align 1
@anon.string.10 = private unnamed_addr constant [2 x i8] c"g\00", align 1
@anon.string.11 = private unnamed_addr constant [9 x i8] c"Doughnut\00", align 1
@anon.string.12 = private unnamed_addr constant [4 x i8] c"pcs\00", align 1
@anon.array.0 = private unnamed_addr constant [3 x %struct.ShoppingItem] [%struct.ShoppingItem { ptr @anon.string.7, double 1.000000e+02, ptr @anon.string.8 }, %struct.ShoppingItem { ptr @anon.string.9, double 1.255000e+02, ptr @anon.string.10 }, %struct.ShoppingItem { ptr @anon.string.11, double 6.000000e+00, ptr @anon.string.12 }]
@anon.string.13 = private unnamed_addr constant [13 x i8] c"Another Cart\00", align 1
@printf.str.0 = private unnamed_addr constant [26 x i8] c"Shopping cart item 1: %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [30 x i8] c"Another cart item 2 unit: %s\0A\00", align 1

define private %struct.ShoppingCart @_Z15newShoppingCartv() {
  %result = alloca %struct.ShoppingCart, align 8
  %items = alloca [3 x %struct.ShoppingItem], align 8
  %1 = alloca %struct.ShoppingCart, align 8
  store [3 x %struct.ShoppingItem] [%struct.ShoppingItem { ptr @0, double 0.000000e+00, ptr @1 }, %struct.ShoppingItem { ptr @0, double 0.000000e+00, ptr @1 }, %struct.ShoppingItem { ptr @0, double 0.000000e+00, ptr @1 }], ptr %items, align 8
  %2 = getelementptr inbounds [3 x %struct.ShoppingItem], ptr %items, i32 0, i32 0
  store %struct.ShoppingItem { ptr @anon.string.0, double 1.000000e+02, ptr @anon.string.1 }, ptr %2, align 8
  %3 = getelementptr inbounds [3 x %struct.ShoppingItem], ptr %items, i32 0, i32 1
  store %struct.ShoppingItem { ptr @anon.string.2, double 1.255000e+02, ptr @anon.string.3 }, ptr %3, align 8
  %4 = getelementptr inbounds [3 x %struct.ShoppingItem], ptr %items, i32 0, i32 2
  store %struct.ShoppingItem { ptr @anon.string.4, double 6.000000e+00, ptr @anon.string.5 }, ptr %4, align 8
  store ptr @anon.string.6, ptr %1, align 8
  %5 = load [3 x %struct.ShoppingItem], ptr %items, align 8
  %6 = getelementptr inbounds %struct.ShoppingCart, ptr %1, i32 0, i32 1
  store [3 x %struct.ShoppingItem] %5, ptr %6, align 8
  %7 = load %struct.ShoppingCart, ptr %1, align 8
  ret %struct.ShoppingCart %7
}

define private %struct.ShoppingCart @_Z19anotherShoppingCartv() {
  %result = alloca %struct.ShoppingCart, align 8
  %items = alloca [3 x %struct.ShoppingItem], align 8
  %1 = alloca %struct.ShoppingCart, align 8
  store [3 x %struct.ShoppingItem] [%struct.ShoppingItem { ptr @anon.string.7, double 1.000000e+02, ptr @anon.string.8 }, %struct.ShoppingItem { ptr @anon.string.9, double 1.255000e+02, ptr @anon.string.10 }, %struct.ShoppingItem { ptr @anon.string.11, double 6.000000e+00, ptr @anon.string.12 }], ptr %items, align 8
  store ptr @anon.string.13, ptr %1, align 8
  %2 = load [3 x %struct.ShoppingItem], ptr %items, align 8
  %3 = getelementptr inbounds %struct.ShoppingCart, ptr %1, i32 0, i32 1
  store [3 x %struct.ShoppingItem] %2, ptr %3, align 8
  %4 = load %struct.ShoppingCart, ptr %1, align 8
  ret %struct.ShoppingCart %4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %shoppingCart = alloca %struct.ShoppingCart, align 8
  store i32 0, ptr %result, align 4
  %1 = call %struct.ShoppingCart @_Z15newShoppingCartv()
  store %struct.ShoppingCart %1, ptr %shoppingCart, align 8
  %items_addr = getelementptr inbounds %struct.ShoppingCart, ptr %shoppingCart, i32 0, i32 1
  %2 = getelementptr inbounds [3 x %struct.ShoppingItem], ptr %items_addr, i32 0, i32 1
  %name_addr = getelementptr inbounds %struct.ShoppingItem, ptr %2, i32 0, i32 0
  %3 = load ptr, ptr %name_addr, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %3)
  %5 = call %struct.ShoppingCart @_Z19anotherShoppingCartv()
  store %struct.ShoppingCart %5, ptr %shoppingCart, align 8
  %items_addr1 = getelementptr inbounds %struct.ShoppingCart, ptr %shoppingCart, i32 0, i32 1
  %6 = getelementptr inbounds [3 x %struct.ShoppingItem], ptr %items_addr1, i32 0, i32 2
  %unit_addr = getelementptr inbounds %struct.ShoppingItem, ptr %6, i32 0, i32 2
  %7 = load ptr, ptr %unit_addr, align 8
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %7)
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
