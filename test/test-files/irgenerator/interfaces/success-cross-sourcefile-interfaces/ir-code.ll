; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Car = type { %interface.Driveable, i1 }
%interface.Driveable = type { ptr }

$_ZTS3Car = comdat any

$_ZTI3Car = comdat any

$_ZTV3Car = comdat any

@_ZTS3Car = private constant [5 x i8] c"3Car\00", comdat, align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI9Driveable = external global ptr
@_ZTI3Car = private constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS3Car, ptr @_ZTI9Driveable }, comdat, align 8
@_ZTV3Car = private unnamed_addr constant { [4 x ptr] } { [4 x ptr] [ptr null, ptr @_ZTI3Car, ptr @_ZN3Car5driveEi, ptr @_ZN3Car9isDrivingEv] }, comdat, align 8
@printf.str.0 = private unnamed_addr constant [15 x i8] c"Is driving: %d\00", align 4

define private void @_ZN3Car4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [4 x ptr] }, ptr @_ZTV3Car, i64 0, i32 0, i32 2), ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.Car, ptr %2, i32 0, i32 1
  store i1 false, ptr %3, align 1
  %4 = load ptr, ptr %this, align 8
  %driving.addr = getelementptr inbounds %struct.Car, ptr %4, i64 0, i32 1
  store i1 false, ptr %driving.addr, align 1
  ret void
}

define private void @_ZN3Car5driveEi(ptr noundef nonnull align 8 dereferenceable(16) %0, i32 noundef %1) {
  %this = alloca ptr, align 8
  %_param = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %_param, align 4
  %3 = load ptr, ptr %this, align 8
  %driving.addr = getelementptr inbounds %struct.Car, ptr %3, i64 0, i32 1
  store i1 true, ptr %driving.addr, align 1
  ret void
}

define private noundef zeroext i1 @_ZN3Car9isDrivingEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %result = alloca i1, align 1
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %driving.addr = getelementptr inbounds %struct.Car, ptr %2, i64 0, i32 1
  %3 = load i1, ptr %driving.addr, align 1
  ret i1 %3
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %car = alloca %struct.Car, align 8
  %driveable = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN3Car4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %car)
  store ptr %car, ptr %driveable, align 8
  %1 = load ptr, ptr %driveable, align 8
  %vtable.addr = load ptr, ptr %1, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  call void %fct(ptr noundef nonnull align 8 dereferenceable(8) %1, i32 noundef 12)
  %2 = load ptr, ptr %driveable, align 8
  %vtable.addr1 = load ptr, ptr %2, align 8
  %vfct.addr2 = getelementptr inbounds ptr, ptr %vtable.addr1, i64 1
  %fct3 = load ptr, ptr %vfct.addr2, align 8
  %3 = call noundef zeroext i1 %fct3(ptr noundef nonnull align 8 dereferenceable(8) %2)
  %4 = zext i1 %3 to i32
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
