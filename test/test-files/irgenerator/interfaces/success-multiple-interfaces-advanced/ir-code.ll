; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.String = type { ptr, i64, i64 }
%struct.Car = type { %interface.Driveable, %interface.Comparable, %interface.Named, %struct.String, i32 }
%interface.Driveable = type { ptr }
%interface.Comparable = type { ptr }
%interface.Named = type { ptr }

@_ZTS9Driveable = private constant [11 x i8] c"9Driveable\00", align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI9Driveable = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS9Driveable }, align 8
@_ZTV9Driveable = private unnamed_addr constant { [4 x ptr] } { [4 x ptr] [ptr null, ptr @_ZTI9Driveable, ptr null, ptr null] }, align 8
@_ZTS10ComparableI3CarE = private constant [13 x i8] c"10Comparable\00", align 4
@_ZTI10ComparableI3CarE = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS10ComparableI3CarE }, align 8
@_ZTV10ComparableI3CarE = private unnamed_addr constant { [4 x ptr] } { [4 x ptr] [ptr null, ptr @_ZTI10ComparableI3CarE, ptr null, ptr null] }, align 8
@_ZTS5Named = private constant [7 x i8] c"5Named\00", align 4
@_ZTI5Named = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5Named }, align 8
@_ZTV5Named = private unnamed_addr constant { [6 x ptr] } { [6 x ptr] [ptr null, ptr @_ZTI5Named, ptr null, ptr null, ptr null, ptr null] }, align 8
@printf.str.0 = private unnamed_addr constant [30 x i8] c"%s (named: %d, distance: %d)\0A\00", align 4
@_ZTS3Car = private constant [5 x i8] c"3Car\00", align 4
@_ZTI3Car = private constant { ptr, ptr, ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS3Car, ptr @_ZTI9Driveable, ptr @_ZTI10ComparableI3CarE, ptr @_ZTI5Named }, align 8
@_ZTV3Car = private unnamed_addr constant { [4 x ptr], [3 x ptr], [6 x ptr] } { [4 x ptr] [ptr null, ptr @_ZTI3Car, ptr @_ZN3Car5driveEi, ptr @_ZN3Car11getDistanceEv], [3 x ptr] [ptr null, ptr @_ZTI3Car, ptr @_ZThn8_N3Car9compareToEPK3Car], [6 x ptr] [ptr null, ptr @_ZTI3Car, ptr @_ZThn16_N3Car7getNameEv, ptr @_ZThn16_N3Car7isNamedEb, ptr @_ZThn16_N3Car11getDistanceEv, ptr @_ZN5Named12printSummaryEv] }, align 8
@printf.str.1 = private unnamed_addr constant [14 x i8] c"Distance: %d\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [6 x i8] c"Car 1\00", align 4
@anon.string.1 = private unnamed_addr constant [6 x i8] c"Car 2\00", align 4
@printf.str.2 = private unnamed_addr constant [13 x i8] c"Compare: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [13 x i8] c"Compare: %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [10 x i8] c"Name: %s\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [14 x i8] c"Is named: %d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [25 x i8] c"Distance via Named*: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN5Named12printSummaryEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  %name = alloca %struct.String, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %vtable.addr = load ptr, ptr %2, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  call void %fct(ptr dead_on_unwind writable sret(%struct.String) align 8 %name, ptr noundef nonnull align 8 dereferenceable(8) %2)
  %3 = call noundef ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %name)
  %4 = load ptr, ptr %this, align 8
  %vtable.addr1 = load ptr, ptr %4, align 8
  %vfct.addr2 = getelementptr inbounds ptr, ptr %vtable.addr1, i64 1
  %fct3 = load ptr, ptr %vfct.addr2, align 8
  %5 = call noundef zeroext i1 %fct3(ptr noundef nonnull align 8 dereferenceable(8) %4, i1 noundef zeroext true)
  %6 = zext i1 %5 to i32
  %7 = load ptr, ptr %this, align 8
  %vtable.addr4 = load ptr, ptr %7, align 8
  %vfct.addr5 = getelementptr inbounds ptr, ptr %vtable.addr4, i64 2
  %fct6 = load ptr, ptr %vfct.addr5, align 8
  %8 = call noundef i32 %fct6(ptr noundef nonnull align 8 dereferenceable(8) %7)
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef %3, i32 noundef %6, i32 noundef %8)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %name)
  ret void
}

declare ptr @_ZN6String6getRawEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN3Car4dtorEv(ptr noundef nonnull align 8 dereferenceable(56) %0) #2 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds nuw %struct.Car, ptr %2, i32 0, i32 3
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN3Car4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(56) %0, ptr noundef %1) #0 {
  %this = alloca ptr, align 8
  %name = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %name, align 8
  %3 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [4 x ptr], [3 x ptr], [6 x ptr] }, ptr @_ZTV3Car, i64 0, i32 0, i32 2), ptr %3, align 8
  %4 = getelementptr inbounds nuw %struct.Car, ptr %3, i32 0, i32 1
  store ptr getelementptr inbounds ({ [4 x ptr], [3 x ptr], [6 x ptr] }, ptr @_ZTV3Car, i64 0, i32 1, i32 2), ptr %4, align 8
  %5 = getelementptr inbounds nuw %struct.Car, ptr %3, i32 0, i32 2
  store ptr getelementptr inbounds ({ [4 x ptr], [3 x ptr], [6 x ptr] }, ptr @_ZTV3Car, i64 0, i32 2, i32 2), ptr %5, align 8
  %6 = getelementptr inbounds nuw %struct.Car, ptr %3, i32 0, i32 3
  call void @_ZN6String4ctorEv(ptr noundef nonnull align 8 dereferenceable(24) %6)
  %7 = getelementptr inbounds nuw %struct.Car, ptr %3, i32 0, i32 4
  store i32 0, ptr %7, align 4
  %8 = load ptr, ptr %this, align 8
  %name.addr = getelementptr inbounds %struct.Car, ptr %8, i64 0, i32 3
  %9 = load ptr, ptr %name, align 8
  call void @_Z9op.assignR6StringRK6String(ptr %name.addr, ptr %9)
  %10 = load ptr, ptr %this, align 8
  %distance.addr = getelementptr inbounds %struct.Car, ptr %10, i64 0, i32 4
  store i32 0, ptr %distance.addr, align 4
  ret void
}

declare void @_ZN6String4ctorEv(ptr noundef nonnull align 8 dereferenceable(24))

declare void @_Z9op.assignR6StringRK6String(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN3Car5driveEi(ptr noundef nonnull align 8 dereferenceable(56) %0, i32 noundef %1) #0 {
  %this = alloca ptr, align 8
  %distance = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %distance, align 4
  %3 = load ptr, ptr %this, align 8
  %distance.addr = getelementptr inbounds %struct.Car, ptr %3, i64 0, i32 4
  %4 = load i32, ptr %distance, align 4
  %5 = load i32, ptr %distance.addr, align 4
  %6 = add nsw i32 %5, %4
  store i32 %6, ptr %distance.addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN3Car11getDistanceEv(ptr noundef nonnull align 8 dereferenceable(56) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %distance.addr = getelementptr inbounds %struct.Car, ptr %2, i64 0, i32 4
  %3 = load i32, ptr %distance.addr, align 4
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN3Car9compareToEPK3Car(ptr noundef nonnull align 8 dereferenceable(56) %0, ptr noundef align 8 %1) #0 {
  %this = alloca ptr, align 8
  %other = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %other, align 8
  %3 = load ptr, ptr %this, align 8
  %distance.addr = getelementptr inbounds %struct.Car, ptr %3, i64 0, i32 4
  %4 = load ptr, ptr %other, align 8
  %distance.addr1 = getelementptr inbounds %struct.Car, ptr %4, i64 0, i32 4
  %5 = load i32, ptr %distance.addr1, align 4
  %6 = load i32, ptr %distance.addr, align 4
  %7 = sub nsw i32 %6, %5
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN3Car7getNameEv(ptr dead_on_unwind noalias writable sret(%struct.String) align 8 %0, ptr noundef nonnull align 8 dereferenceable(56) %1) #0 {
  %this = alloca ptr, align 8
  store ptr %1, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  %name.addr = getelementptr inbounds %struct.Car, ptr %3, i64 0, i32 3
  call void @_ZN6String4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(24) %0, ptr %name.addr)
  ret void
}

declare void @_ZN6String4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(24), ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef zeroext i1 @_ZN3Car7isNamedEb(ptr noundef nonnull align 8 dereferenceable(56) %0, i1 noundef zeroext %1) #0 {
  %this = alloca ptr, align 8
  %expected = alloca i1, align 1
  store ptr %0, ptr %this, align 8
  store i1 %1, ptr %expected, align 1
  %3 = load ptr, ptr %this, align 8
  %4 = getelementptr inbounds nuw %struct.Car, ptr %3, i32 0, i32 3
  %5 = call noundef zeroext i1 @_ZN6String7isEmptyEv(ptr noundef nonnull align 8 dereferenceable(24) %4)
  %6 = xor i1 %5, true
  %7 = load i1, ptr %expected, align 1
  %8 = icmp eq i1 %6, %7
  ret i1 %8
}

declare i1 @_ZN6String7isEmptyEv(ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z13printDistanceP9Driveable(ptr noundef align 8 %0) #0 {
  %driveable = alloca ptr, align 8
  store ptr %0, ptr %driveable, align 8
  %2 = load ptr, ptr %driveable, align 8
  %vtable.addr = load ptr, ptr %2, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 1
  %fct = load ptr, ptr %vfct.addr, align 8
  %3 = call noundef i32 %fct(ptr noundef nonnull align 8 dereferenceable(8) %2)
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z7compareP10ComparableI3CarEPK3Car(ptr noundef align 8 %0, ptr noundef align 8 %1) #0 {
  %comparable = alloca ptr, align 8
  %other = alloca ptr, align 8
  store ptr %0, ptr %comparable, align 8
  store ptr %1, ptr %other, align 8
  %3 = load ptr, ptr %comparable, align 8
  %4 = load ptr, ptr %other, align 8
  %vtable.addr = load ptr, ptr %3, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  %5 = call noundef i32 %fct(ptr noundef nonnull align 8 dereferenceable(8) %3, ptr noundef align 8 %4)
  ret i32 %5
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #3 {
  %car1 = alloca %struct.Car, align 8
  %1 = alloca %struct.String, align 8
  %car2 = alloca %struct.Car, align 8
  %2 = alloca %struct.String, align 8
  %driveable = alloca ptr, align 8
  %comparable = alloca ptr, align 8
  %named = alloca ptr, align 8
  %name = alloca %struct.String, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr noundef @anon.string.0)
  call void @_ZN3Car4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(56) %car1, ptr noundef %1)
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr noundef @anon.string.1)
  call void @_ZN3Car4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(56) %car2, ptr noundef %2)
  store ptr %car1, ptr %driveable, align 8
  %3 = load ptr, ptr %driveable, align 8
  %vtable.addr = load ptr, ptr %3, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  call void %fct(ptr noundef nonnull align 8 dereferenceable(8) %3, i32 noundef 30)
  call void @_ZN3Car5driveEi(ptr noundef nonnull align 8 dereferenceable(56) %car2, i32 noundef 12)
  call void @_Z13printDistanceP9Driveable(ptr noundef align 8 %car1)
  call void @_Z13printDistanceP9Driveable(ptr noundef align 8 %car2)
  %4 = getelementptr inbounds nuw %struct.Car, ptr %car1, i32 0, i32 1
  %5 = icmp eq ptr %car1, null
  %6 = select i1 %5, ptr null, ptr %4
  %7 = call noundef i32 @_Z7compareP10ComparableI3CarEPK3Car(ptr noundef align 8 %6, ptr noundef align 8 %car2)
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %7)
  %9 = getelementptr inbounds nuw %struct.Car, ptr %car2, i32 0, i32 1
  %10 = icmp eq ptr %car2, null
  %11 = select i1 %10, ptr null, ptr %9
  store ptr %11, ptr %comparable, align 8
  %12 = load ptr, ptr %comparable, align 8
  %vtable.addr1 = load ptr, ptr %12, align 8
  %vfct.addr2 = getelementptr inbounds ptr, ptr %vtable.addr1, i64 0
  %fct3 = load ptr, ptr %vfct.addr2, align 8
  %13 = call noundef i32 %fct3(ptr noundef nonnull align 8 dereferenceable(8) %12, ptr noundef align 8 %car1)
  %14 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %13)
  %15 = getelementptr inbounds nuw %struct.Car, ptr %car1, i32 0, i32 2
  %16 = icmp eq ptr %car1, null
  %17 = select i1 %16, ptr null, ptr %15
  store ptr %17, ptr %named, align 8
  %18 = load ptr, ptr %named, align 8
  %vtable.addr4 = load ptr, ptr %18, align 8
  %vfct.addr5 = getelementptr inbounds ptr, ptr %vtable.addr4, i64 0
  %fct6 = load ptr, ptr %vfct.addr5, align 8
  call void %fct6(ptr dead_on_unwind writable sret(%struct.String) align 8 %name, ptr noundef nonnull align 8 dereferenceable(8) %18)
  %19 = call noundef ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %name)
  %20 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, ptr noundef %19)
  %21 = load ptr, ptr %named, align 8
  %vtable.addr7 = load ptr, ptr %21, align 8
  %vfct.addr8 = getelementptr inbounds ptr, ptr %vtable.addr7, i64 1
  %fct9 = load ptr, ptr %vfct.addr8, align 8
  %22 = call noundef zeroext i1 %fct9(ptr noundef nonnull align 8 dereferenceable(8) %21, i1 noundef zeroext true)
  %23 = zext i1 %22 to i32
  %24 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %23)
  %25 = load ptr, ptr %named, align 8
  %vtable.addr10 = load ptr, ptr %25, align 8
  %vfct.addr11 = getelementptr inbounds ptr, ptr %vtable.addr10, i64 2
  %fct12 = load ptr, ptr %vfct.addr11, align 8
  %26 = call noundef i32 %fct12(ptr noundef nonnull align 8 dereferenceable(8) %25)
  %27 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i32 noundef %26)
  %28 = load ptr, ptr %named, align 8
  %vtable.addr13 = load ptr, ptr %28, align 8
  %vfct.addr14 = getelementptr inbounds ptr, ptr %vtable.addr13, i64 3
  %fct15 = load ptr, ptr %vfct.addr14, align 8
  call void %fct15(ptr noundef nonnull align 8 dereferenceable(8) %28)
  %29 = getelementptr inbounds nuw %struct.Car, ptr %car2, i32 0, i32 2
  call void @_ZN5Named12printSummaryEv(ptr noundef nonnull align 8 dereferenceable(8) %29)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %name)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %2)
  call void @_ZN3Car4dtorEv(ptr noundef nonnull align 8 dereferenceable(56) %car2)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %1)
  call void @_ZN3Car4dtorEv(ptr noundef nonnull align 8 dereferenceable(56) %car1)
  ret i32 0
}

declare void @_ZN6String4ctorEPKc(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZThn8_N3Car9compareToEPK3Car(ptr noundef nonnull %0, ptr noundef %1) unnamed_addr #0 {
  %this.adjusted = getelementptr inbounds i8, ptr %0, i64 -8
  %3 = call noundef i32 @_ZN3Car9compareToEPK3Car(ptr noundef nonnull align 8 dereferenceable(56) %this.adjusted, ptr noundef align 8 %1)
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZThn16_N3Car7getNameEv(ptr dead_on_unwind noalias writable sret(%struct.String) align 8 %0, ptr noundef nonnull %1) unnamed_addr #0 {
  %this.adjusted = getelementptr inbounds i8, ptr %1, i64 -16
  call void @_ZN3Car7getNameEv(ptr dead_on_unwind writable sret(%struct.String) align 8 %0, ptr noundef nonnull align 8 dereferenceable(56) %this.adjusted)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef zeroext i1 @_ZThn16_N3Car7isNamedEb(ptr noundef nonnull %0, i1 noundef zeroext %1) unnamed_addr #0 {
  %this.adjusted = getelementptr inbounds i8, ptr %0, i64 -16
  %3 = call noundef zeroext i1 @_ZN3Car7isNamedEb(ptr noundef nonnull align 8 dereferenceable(56) %this.adjusted, i1 noundef zeroext %1)
  ret i1 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZThn16_N3Car11getDistanceEv(ptr noundef nonnull %0) unnamed_addr #0 {
  %this.adjusted = getelementptr inbounds i8, ptr %0, i64 -16
  %2 = call noundef i32 @_ZN3Car11getDistanceEv(ptr noundef nonnull align 8 dereferenceable(56) %this.adjusted)
  ret i32 %2
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { mustprogress noinline nounwind optnone uwtable }
attributes #3 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
