; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Box = type { ptr }
%struct.Box.0 = type { ptr }
%struct.ArrayIterator = type { %interface.IIterator, ptr, i64, i64 }
%interface.IIterator = type { ptr }
%struct.ArrayIterator.1 = type { %interface.IIterator, ptr, i64, i64 }

@printf.str.0 = private unnamed_addr constant [15 x i8] c"Const box: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [9 x i8] c"Box: %d\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [2 x i8] c"a\00", align 4
@anon.string.1 = private unnamed_addr constant [2 x i8] c"b\00", align 4
@anon.array.0 = private unnamed_addr constant [2 x ptr] [ptr @anon.string.0, ptr @anon.string.1]
@printf.str.2 = private unnamed_addr constant [16 x i8] c"Const item: %s\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [2 x i8] c"c\00", align 4
@anon.string.3 = private unnamed_addr constant [2 x i8] c"d\00", align 4
@anon.array.1 = private unnamed_addr constant [2 x ptr] [ptr @anon.string.2, ptr @anon.string.3]
@printf.str.3 = private unnamed_addr constant [10 x i8] c"Item: %s\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [17 x i8] c"Const deref: %d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [11 x i8] c"Deref: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN3BoxIKiE4ctorEPKi(ptr noundef nonnull align 8 dereferenceable(8) %0, ptr noundef align 4 %1) #0 {
  %this = alloca ptr, align 8
  %ptr = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %ptr, align 8
  %3 = load ptr, ptr %this, align 8
  store ptr null, ptr %3, align 8
  %4 = load ptr, ptr %this, align 8
  %ptr.addr = getelementptr inbounds %struct.Box, ptr %4, i64 0, i32 0
  %5 = load ptr, ptr %ptr, align 8
  store ptr %5, ptr %ptr.addr, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN3BoxIiE4ctorEPi(ptr noundef nonnull align 8 dereferenceable(8) %0, ptr noundef align 4 %1) #0 {
  %this = alloca ptr, align 8
  %ptr = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %ptr, align 8
  %3 = load ptr, ptr %this, align 8
  store ptr null, ptr %3, align 8
  %4 = load ptr, ptr %this, align 8
  %ptr.addr = getelementptr inbounds %struct.Box.0, ptr %4, i64 0, i32 0
  %5 = load ptr, ptr %ptr, align 8
  store ptr %5, ptr %ptr.addr, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_ZN3BoxIKiE3getEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %ptr.addr = getelementptr inbounds %struct.Box, ptr %2, i64 0, i32 0
  %3 = load ptr, ptr %ptr.addr, align 8
  ret ptr %3
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_ZN3BoxIiE3getEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %ptr.addr = getelementptr inbounds %struct.Box.0, ptr %2, i64 0, i32 0
  %3 = load ptr, ptr %ptr.addr, align 8
  ret ptr %3
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z5derefIKiERKiPKi(ptr noundef align 4 %0) #0 {
  %ptr = alloca ptr, align 8
  store ptr %0, ptr %ptr, align 8
  %2 = load ptr, ptr %ptr, align 8
  ret ptr %2
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z5derefIiERiPi(ptr noundef align 4 %0) #0 {
  %ptr = alloca ptr, align 8
  store ptr %0, ptr %ptr, align 8
  %2 = load ptr, ptr %ptr, align 8
  ret ptr %2
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #1 {
  %a = alloca i32, align 4
  %constBox = alloca %struct.Box, align 8
  %b = alloca i32, align 4
  %box = alloca %struct.Box.0, align 8
  %boxValue = alloca ptr, align 8
  %constItems = alloca [2 x ptr], align 8
  %1 = alloca %struct.ArrayIterator, align 8
  %item = alloca ptr, align 8
  %2 = alloca %struct.ArrayIterator.1, align 8
  %item1 = alloca ptr, align 8
  %derefValue = alloca ptr, align 8
  store i32 1, ptr %a, align 4
  call void @_ZN3BoxIKiE4ctorEPKi(ptr noundef nonnull align 8 dereferenceable(8) %constBox, ptr noundef align 4 %a)
  %3 = call noundef ptr @_ZN3BoxIKiE3getEv(ptr noundef nonnull align 8 dereferenceable(8) %constBox)
  %4 = load i32, ptr %3, align 4
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %4)
  store i32 2, ptr %b, align 4
  call void @_ZN3BoxIiE4ctorEPi(ptr noundef nonnull align 8 dereferenceable(8) %box, ptr noundef align 4 %b)
  %6 = call noundef ptr @_ZN3BoxIiE3getEv(ptr noundef nonnull align 8 dereferenceable(8) %box)
  store ptr %6, ptr %boxValue, align 8
  %7 = load ptr, ptr %boxValue, align 8
  store i32 3, ptr %7, align 4
  %8 = load i32, ptr %b, align 4
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %8)
  store [2 x ptr] [ptr @anon.string.0, ptr @anon.string.1], ptr %constItems, align 8
  call void @_Z7iterateIKPKcE13ArrayIteratorIKPKcEPKPKcm(ptr dead_on_unwind writable sret(%struct.ArrayIterator) align 8 %1, ptr %constItems, i64 2)
  br label %foreach.head.L41

foreach.head.L41:                                 ; preds = %foreach.tail.L41, %0
  %10 = call i1 @_ZN13ArrayIteratorIKPKcE7isValidEv(ptr %1)
  br i1 %10, label %foreach.body.L41, label %foreach.exit.L41

foreach.body.L41:                                 ; preds = %foreach.head.L41
  %11 = call ptr @_ZN13ArrayIteratorIKPKcE3getEv(ptr %1)
  %12 = load ptr, ptr %11, align 8
  store ptr %12, ptr %item, align 8
  %13 = load ptr, ptr %item, align 8
  %14 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr noundef %13)
  br label %foreach.tail.L41

foreach.tail.L41:                                 ; preds = %foreach.body.L41
  call void @_ZN13ArrayIteratorIKPKcE4nextEv(ptr %1)
  br label %foreach.head.L41

foreach.exit.L41:                                 ; preds = %foreach.head.L41
  call void @_Z7iterateIPKcE13ArrayIteratorIPKcEPPKcm(ptr dead_on_unwind writable sret(%struct.ArrayIterator.1) align 8 %2, ptr @anon.array.1, i64 2)
  br label %foreach.head.L44

foreach.head.L44:                                 ; preds = %foreach.tail.L44, %foreach.exit.L41
  %15 = call i1 @_ZN13ArrayIteratorIPKcE7isValidEv(ptr %2)
  br i1 %15, label %foreach.body.L44, label %foreach.exit.L44

foreach.body.L44:                                 ; preds = %foreach.head.L44
  %16 = call ptr @_ZN13ArrayIteratorIPKcE3getEv(ptr %2)
  %17 = load ptr, ptr %16, align 8
  store ptr %17, ptr %item1, align 8
  %18 = load ptr, ptr %item1, align 8
  %19 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, ptr noundef %18)
  br label %foreach.tail.L44

foreach.tail.L44:                                 ; preds = %foreach.body.L44
  call void @_ZN13ArrayIteratorIPKcE4nextEv(ptr %2)
  br label %foreach.head.L44

foreach.exit.L44:                                 ; preds = %foreach.head.L44
  %20 = call noundef ptr @_Z5derefIKiERKiPKi(ptr noundef align 4 %a)
  %21 = load i32, ptr %20, align 4
  %22 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %21)
  %23 = call noundef ptr @_Z5derefIiERiPi(ptr noundef align 4 %b)
  store ptr %23, ptr %derefValue, align 8
  %24 = load ptr, ptr %derefValue, align 8
  store i32 4, ptr %24, align 4
  %25 = load i32, ptr %b, align 4
  %26 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %25)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

declare void @_Z7iterateIKPKcE13ArrayIteratorIKPKcEPKPKcm(ptr dead_on_unwind noalias writable sret(%struct.ArrayIterator) align 8, ptr, i64)

declare i1 @_ZN13ArrayIteratorIKPKcE7isValidEv(ptr)

declare ptr @_ZN13ArrayIteratorIKPKcE3getEv(ptr)

declare void @_ZN13ArrayIteratorIKPKcE4nextEv(ptr)

declare void @_Z7iterateIPKcE13ArrayIteratorIPKcEPPKcm(ptr dead_on_unwind noalias writable sret(%struct.ArrayIterator.1) align 8, ptr, i64)

declare i1 @_ZN13ArrayIteratorIPKcE7isValidEv(ptr)

declare ptr @_ZN13ArrayIteratorIPKcE3getEv(ptr)

declare void @_ZN13ArrayIteratorIPKcE4nextEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
