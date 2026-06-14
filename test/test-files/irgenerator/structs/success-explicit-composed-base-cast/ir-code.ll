; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Base = type { i32 }
%struct.Derived = type { %interface.IMarker, %struct.Base, i32 }
%interface.IMarker = type { ptr }
%struct.Leaf = type { %interface.IMarker, %struct.Derived, i32 }

@_ZTS7IMarker = private constant [9 x i8] c"7IMarker\00", align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI7IMarker = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS7IMarker }, align 8
@_ZTV7IMarker = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI7IMarker, ptr null] }, align 8
@_ZTS7Derived = private constant [9 x i8] c"7Derived\00", align 4
@_ZTI7Derived = private constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS7Derived, ptr @_ZTI7IMarker }, align 8
@_ZTV7Derived = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI7Derived, ptr @_ZN7Derived6markerEv] }, align 8
@_ZTS4Leaf = private constant [6 x i8] c"4Leaf\00", align 4
@_ZTI4Leaf = private constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS4Leaf, ptr @_ZTI7IMarker }, align 8
@_ZTV4Leaf = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI4Leaf, ptr @_ZN4Leaf6markerEv] }, align 8
@printf.str.0 = private unnamed_addr constant [20 x i8] c"derived->base:  %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [20 x i8] c"leaf->base:     %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [20 x i8] c"derived->iface: %d\0A\00", align 4

define private void @_ZN4Base4initEi(ptr noundef nonnull align 4 dereferenceable(4) %0, i32 noundef %1) {
  %this = alloca ptr, align 8
  %v = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %v, align 4
  %3 = load ptr, ptr %this, align 8
  %baseVal.addr = getelementptr inbounds %struct.Base, ptr %3, i64 0, i32 0
  %4 = load i32, ptr %v, align 4
  store i32 %4, ptr %baseVal.addr, align 4
  ret void
}

define private noundef i32 @_ZN4Base3getEv(ptr noundef nonnull align 4 dereferenceable(4) %0) {
  %result = alloca i32, align 4
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %baseVal.addr = getelementptr inbounds %struct.Base, ptr %2, i64 0, i32 0
  %3 = load i32, ptr %baseVal.addr, align 4
  ret i32 %3
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN7Derived4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV7Derived, i64 0, i32 0, i32 2), ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.Derived, ptr %2, i32 0, i32 2
  store i32 0, ptr %3, align 4
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN7Derived4ctorERK7Derived(ptr noundef nonnull align 8 dereferenceable(16) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %3, ptr %1, i64 8, i1 false)
  %4 = getelementptr inbounds nuw %struct.Derived, ptr %1, i32 0, i32 1
  %5 = getelementptr inbounds nuw %struct.Derived, ptr %3, i32 0, i32 1
  call void @llvm.memcpy.p0.p0.i64(ptr %5, ptr %4, i64 4, i1 false)
  %6 = getelementptr inbounds nuw %struct.Derived, ptr %1, i32 0, i32 2
  %7 = getelementptr inbounds nuw %struct.Derived, ptr %3, i32 0, i32 2
  call void @llvm.memcpy.p0.p0.i64(ptr %7, ptr %6, i64 4, i1 false)
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #1

define private void @_ZN7Derived4initEii(ptr noundef nonnull align 8 dereferenceable(16) %0, i32 noundef %1, i32 noundef %2) {
  %this = alloca ptr, align 8
  %b = alloca i32, align 4
  %d = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %b, align 4
  store i32 %2, ptr %d, align 4
  %4 = load ptr, ptr %this, align 8
  %5 = getelementptr inbounds nuw %struct.Derived, ptr %4, i32 0, i32 1
  %6 = load i32, ptr %b, align 4
  call void @_ZN4Base4initEi(ptr noundef nonnull align 4 dereferenceable(4) %5, i32 noundef %6)
  %7 = load ptr, ptr %this, align 8
  %derivedVal.addr = getelementptr inbounds %struct.Derived, ptr %7, i64 0, i32 2
  %8 = load i32, ptr %d, align 4
  store i32 %8, ptr %derivedVal.addr, align 4
  ret void
}

define private noundef i32 @_ZN7Derived6markerEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %result = alloca i32, align 4
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %derivedVal.addr = getelementptr inbounds %struct.Derived, ptr %2, i64 0, i32 2
  %3 = load i32, ptr %derivedVal.addr, align 4
  ret i32 %3
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN4Leaf4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV4Leaf, i64 0, i32 0, i32 2), ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.Leaf, ptr %2, i32 0, i32 1
  call void @_ZN7Derived4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %3)
  %4 = getelementptr inbounds nuw %struct.Leaf, ptr %2, i32 0, i32 2
  store i32 0, ptr %4, align 4
  ret void
}

define private void @_ZN4Leaf4initEiii(ptr noundef nonnull align 8 dereferenceable(32) %0, i32 noundef %1, i32 noundef %2, i32 noundef %3) {
  %this = alloca ptr, align 8
  %b = alloca i32, align 4
  %d = alloca i32, align 4
  %l = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %b, align 4
  store i32 %2, ptr %d, align 4
  store i32 %3, ptr %l, align 4
  %5 = load ptr, ptr %this, align 8
  %6 = getelementptr inbounds nuw %struct.Leaf, ptr %5, i32 0, i32 1
  %7 = load i32, ptr %b, align 4
  %8 = load i32, ptr %d, align 4
  call void @_ZN7Derived4initEii(ptr noundef nonnull align 8 dereferenceable(16) %6, i32 noundef %7, i32 noundef %8)
  %9 = load ptr, ptr %this, align 8
  %leafVal.addr = getelementptr inbounds %struct.Leaf, ptr %9, i64 0, i32 2
  %10 = load i32, ptr %l, align 4
  store i32 %10, ptr %leafVal.addr, align 4
  ret void
}

define private noundef i32 @_ZN4Leaf6markerEv(ptr noundef nonnull align 8 dereferenceable(32) %0) {
  %result = alloca i32, align 4
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %leafVal.addr = getelementptr inbounds %struct.Leaf, ptr %2, i64 0, i32 2
  %3 = load i32, ptr %leafVal.addr, align 4
  ret i32 %3
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 {
  %result = alloca i32, align 4
  %der = alloca %struct.Derived, align 8
  %b = alloca ptr, align 8
  %leaf = alloca %struct.Leaf, align 8
  %lb = alloca ptr, align 8
  %m = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN7Derived4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %der)
  call void @_ZN7Derived4initEii(ptr noundef nonnull align 8 dereferenceable(16) %der, i32 noundef 7, i32 noundef 99)
  %1 = getelementptr inbounds nuw %struct.Derived, ptr %der, i32 0, i32 1
  store ptr %1, ptr %b, align 8
  %2 = load ptr, ptr %b, align 8
  %3 = call noundef i32 @_ZN4Base3getEv(ptr noundef nonnull align 4 dereferenceable(4) %2)
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3)
  call void @_ZN4Leaf4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %leaf)
  call void @_ZN4Leaf4initEiii(ptr noundef nonnull align 8 dereferenceable(32) %leaf, i32 noundef 1, i32 noundef 2, i32 noundef 3)
  %5 = getelementptr inbounds nuw %struct.Leaf, ptr %leaf, i32 0, i32 1
  %6 = getelementptr inbounds nuw %struct.Derived, ptr %5, i32 0, i32 1
  store ptr %6, ptr %lb, align 8
  %7 = load ptr, ptr %lb, align 8
  %8 = call noundef i32 @_ZN4Base3getEv(ptr noundef nonnull align 4 dereferenceable(4) %7)
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %8)
  store ptr %der, ptr %m, align 8
  %10 = load ptr, ptr %m, align 8
  %vtable.addr = load ptr, ptr %10, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  %11 = call noundef i32 %fct(ptr noundef nonnull align 8 dereferenceable(8) %10)
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %11)
  %13 = load i32, ptr %result, align 4
  ret i32 %13
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

attributes #0 = { mustprogress noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #3 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
