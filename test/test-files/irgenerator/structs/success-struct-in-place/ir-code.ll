; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.ShoppingItem = type { ptr, double, ptr }
%struct.ShoppingCart = type { ptr, [3 x %struct.ShoppingItem] }

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 4
@1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 4
@anon.string.0 = private unnamed_addr constant [10 x i8] c"Spaghetti\00", align 4
@anon.string.1 = private unnamed_addr constant [2 x i8] c"g\00", align 4
@anon.string.2 = private unnamed_addr constant [5 x i8] c"Rice\00", align 4
@anon.string.3 = private unnamed_addr constant [2 x i8] c"g\00", align 4
@anon.string.4 = private unnamed_addr constant [9 x i8] c"Doughnut\00", align 4
@anon.string.5 = private unnamed_addr constant [4 x i8] c"pcs\00", align 4
@anon.string.6 = private unnamed_addr constant [14 x i8] c"Shopping Cart\00", align 4
@anon.string.7 = private unnamed_addr constant [10 x i8] c"Spaghetti\00", align 4
@anon.string.8 = private unnamed_addr constant [2 x i8] c"g\00", align 4
@anon.string.9 = private unnamed_addr constant [5 x i8] c"Rice\00", align 4
@anon.string.10 = private unnamed_addr constant [2 x i8] c"g\00", align 4
@anon.string.11 = private unnamed_addr constant [9 x i8] c"Doughnut\00", align 4
@anon.string.12 = private unnamed_addr constant [4 x i8] c"pcs\00", align 4
@anon.array.0 = private unnamed_addr constant [3 x %struct.ShoppingItem] [%struct.ShoppingItem { ptr @anon.string.7, double 1.000000e+02, ptr @anon.string.8 }, %struct.ShoppingItem { ptr @anon.string.9, double 1.255000e+02, ptr @anon.string.10 }, %struct.ShoppingItem { ptr @anon.string.11, double 6.000000e+00, ptr @anon.string.12 }]
@anon.string.13 = private unnamed_addr constant [13 x i8] c"Another Cart\00", align 4
@printf.str.0 = private unnamed_addr constant [26 x i8] c"Shopping cart item 1: %s\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [30 x i8] c"Another cart item 2 unit: %s\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z15newShoppingCartv(ptr dead_on_unwind noalias writable sret(%struct.ShoppingCart) align 8 %0) #0 {
  %items = alloca [3 x %struct.ShoppingItem], align 8
  %2 = alloca %struct.ShoppingCart, align 8
  store [3 x %struct.ShoppingItem] [%struct.ShoppingItem { ptr @0, double 0.000000e+00, ptr @1 }, %struct.ShoppingItem { ptr @0, double 0.000000e+00, ptr @1 }, %struct.ShoppingItem { ptr @0, double 0.000000e+00, ptr @1 }], ptr %items, align 8
  %3 = getelementptr inbounds [3 x %struct.ShoppingItem], ptr %items, i64 0, i32 0
  store %struct.ShoppingItem { ptr @anon.string.0, double 1.000000e+02, ptr @anon.string.1 }, ptr %3, align 8
  %4 = getelementptr inbounds [3 x %struct.ShoppingItem], ptr %items, i64 0, i32 1
  store %struct.ShoppingItem { ptr @anon.string.2, double 1.255000e+02, ptr @anon.string.3 }, ptr %4, align 8
  %5 = getelementptr inbounds [3 x %struct.ShoppingItem], ptr %items, i64 0, i32 2
  store %struct.ShoppingItem { ptr @anon.string.4, double 6.000000e+00, ptr @anon.string.5 }, ptr %5, align 8
  store ptr @anon.string.6, ptr %2, align 8
  %6 = load [3 x %struct.ShoppingItem], ptr %items, align 8
  %7 = getelementptr inbounds nuw %struct.ShoppingCart, ptr %2, i32 0, i32 1
  store [3 x %struct.ShoppingItem] %6, ptr %7, align 8
  %8 = load %struct.ShoppingCart, ptr %2, align 8
  store %struct.ShoppingCart %8, ptr %0, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z19anotherShoppingCartv(ptr dead_on_unwind noalias writable sret(%struct.ShoppingCart) align 8 %0) #0 {
  %items = alloca [3 x %struct.ShoppingItem], align 8
  %2 = alloca %struct.ShoppingCart, align 8
  store [3 x %struct.ShoppingItem] [%struct.ShoppingItem { ptr @anon.string.7, double 1.000000e+02, ptr @anon.string.8 }, %struct.ShoppingItem { ptr @anon.string.9, double 1.255000e+02, ptr @anon.string.10 }, %struct.ShoppingItem { ptr @anon.string.11, double 6.000000e+00, ptr @anon.string.12 }], ptr %items, align 8
  store ptr @anon.string.13, ptr %2, align 8
  %3 = load [3 x %struct.ShoppingItem], ptr %items, align 8
  %4 = getelementptr inbounds nuw %struct.ShoppingCart, ptr %2, i32 0, i32 1
  store [3 x %struct.ShoppingItem] %3, ptr %4, align 8
  %5 = load %struct.ShoppingCart, ptr %2, align 8
  store %struct.ShoppingCart %5, ptr %0, align 8
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %shoppingCart = alloca %struct.ShoppingCart, align 8
  %1 = alloca %struct.ShoppingCart, align 8
  call void @_Z15newShoppingCartv(ptr dead_on_unwind writable sret(%struct.ShoppingCart) align 8 %shoppingCart)
  %items.addr = getelementptr inbounds %struct.ShoppingCart, ptr %shoppingCart, i64 0, i32 1
  %2 = getelementptr inbounds [3 x %struct.ShoppingItem], ptr %items.addr, i64 0, i32 1
  %name.addr = getelementptr inbounds %struct.ShoppingItem, ptr %2, i64 0, i32 0
  %3 = load ptr, ptr %name.addr, align 8
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef %3)
  call void @_Z19anotherShoppingCartv(ptr dead_on_unwind writable sret(%struct.ShoppingCart) align 8 %1)
  call void @llvm.memcpy.p0.p0.i64(ptr %shoppingCart, ptr %1, i64 80, i1 false)
  %items.addr1 = getelementptr inbounds %struct.ShoppingCart, ptr %shoppingCart, i64 0, i32 1
  %5 = getelementptr inbounds [3 x %struct.ShoppingItem], ptr %items.addr1, i64 0, i32 2
  %unit.addr = getelementptr inbounds %struct.ShoppingItem, ptr %5, i64 0, i32 2
  %6 = load ptr, ptr %unit.addr, align 8
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr noundef %6)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #3

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
