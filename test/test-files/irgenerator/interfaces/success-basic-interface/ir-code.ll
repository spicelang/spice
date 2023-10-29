; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Car = type { %interface.Driveable, i1 }
%interface.Driveable = type { ptr }

$_ZTS9Driveable = comdat any

$_ZTI9Driveable = comdat any

$_ZTV9Driveable = comdat any

$_ZTS3Car = comdat any

$_ZTI3Car = comdat any

$_ZTV3Car = comdat any

@_ZTV8TypeInfo = external global ptr
@_ZTS9Driveable = linkonce_odr dso_local constant [11 x i8] c"9Driveable\00", comdat, align 1
@_ZTI9Driveable = linkonce_odr dso_local constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i32 2), ptr @_ZTS9Driveable }, comdat
@_ZTV9Driveable = linkonce_odr dso_local unnamed_addr constant { [4 x ptr] } { [4 x ptr] [ptr null, ptr @_ZTI9Driveable, ptr null, ptr null] }, comdat
@_ZTS3Car = linkonce_odr dso_local constant [5 x i8] c"3Car\00", comdat, align 1
@_ZTI3Car = linkonce_odr dso_local constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i32 2), ptr @_ZTS3Car }, comdat
@_ZTV3Car = linkonce_odr dso_local unnamed_addr constant { [4 x ptr] } { [4 x ptr] [ptr null, ptr @_ZTI3Car, ptr @_ZN3Car5driveEi, ptr @_ZN3Car9isDrivingEv] }, comdat
@printf.str.0 = private unnamed_addr constant [15 x i8] c"Is driving: %d\00", align 1

define dso_local void @_ZN3Car4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store %struct.Car zeroinitializer, ptr %0, align 8
  %2 = load ptr, ptr %this, align 8
  %driving_addr = getelementptr inbounds %struct.Car, ptr %2, i32 0, i32 0
  store i1 false, ptr %driving_addr, align 1
  ret void
}

define dso_local void @_ZN3Car5driveEi(ptr noundef nonnull align 8 dereferenceable(16) %0, i32 %1) {
  %this = alloca ptr, align 8
  %param = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %param, align 4
  %3 = load ptr, ptr %this, align 8
  %driving_addr = getelementptr inbounds %struct.Car, ptr %3, i32 0, i32 0
  store i1 true, ptr %driving_addr, align 1
  ret void
}

define dso_local i1 @_ZN3Car9isDrivingEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %result = alloca i1, align 1
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %driving_addr = getelementptr inbounds %struct.Car, ptr %2, i32 0, i32 0
  %3 = load i1, ptr %driving_addr, align 1
  ret i1 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %car = alloca %struct.Car, align 8
  %driveable = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN3Car4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %car)
  store ptr %car, ptr %driveable, align 8
  %1 = load ptr, ptr %driveable, align 8
  %2 = load ptr, ptr %driveable, align 8
  %vtable.addr = load ptr, ptr %2, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i32 0
  %fct = load ptr, ptr %vfct.addr, align 8
  call void %fct(ptr noundef nonnull align 8 dereferenceable(8) %1, i32 12)
  %3 = load ptr, ptr %driveable, align 8
  %4 = load ptr, ptr %driveable, align 8
  %vtable.addr1 = load ptr, ptr %4, align 8
  %vfct.addr2 = getelementptr inbounds ptr, ptr %vtable.addr1, i32 0
  %fct3 = load ptr, ptr %vfct.addr2, align 8
  %5 = call i1 %fct3(ptr noundef nonnull align 8 dereferenceable(8) %3)
  %6 = zext i1 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %6)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
