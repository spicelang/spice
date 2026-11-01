; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Square = type { %interface.Shape, i32 }
%interface.Shape = type { ptr }
%struct.Counter = type { i32, %struct.Square }

@_ZTS5Shape = private constant [7 x i8] c"5Shape\00", align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI5Shape = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5Shape }, align 8
@_ZTV5Shape = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI5Shape, ptr null] }, align 8
@_ZTS6Square = private constant [8 x i8] c"6Square\00", align 4
@_ZTI6Square = private constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS6Square, ptr @_ZTI5Shape }, align 8
@_ZTV6Square = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI6Square, ptr @_ZN6Square4areaEv] }, align 8
@printf.str.0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.7 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.8 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN6Square4ctorERK6Square(ptr noundef nonnull align 8 dereferenceable(16) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %3, ptr %1, i64 8, i1 false)
  %4 = getelementptr inbounds nuw %struct.Square, ptr %1, i32 0, i32 1
  %5 = getelementptr inbounds nuw %struct.Square, ptr %3, i32 0, i32 1
  call void @llvm.memcpy.p0.p0.i64(ptr %5, ptr %4, i64 4, i1 false)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #1

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN6Square4ctorEi(ptr noundef nonnull align 8 dereferenceable(16) %0, i32 noundef %1) #2 {
  %this = alloca ptr, align 8
  %side = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %side, align 4
  %3 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV6Square, i64 0, i32 0, i32 2), ptr %3, align 8
  %4 = getelementptr inbounds nuw %struct.Square, ptr %3, i32 0, i32 1
  store i32 0, ptr %4, align 4
  %5 = load ptr, ptr %this, align 8
  %side.addr = getelementptr inbounds %struct.Square, ptr %5, i64 0, i32 1
  %6 = load i32, ptr %side, align 4
  store i32 %6, ptr %side.addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN6Square4areaEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #2 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %side.addr = getelementptr inbounds %struct.Square, ptr %2, i64 0, i32 1
  %3 = load ptr, ptr %this, align 8
  %side.addr1 = getelementptr inbounds %struct.Square, ptr %3, i64 0, i32 1
  %4 = load i32, ptr %side.addr1, align 4
  %5 = load i32, ptr %side.addr, align 4
  %6 = mul nsw i32 %5, %4
  ret i32 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN7Counter4ctorEv(ptr noundef nonnull align 8 dereferenceable(24) %0) #2 {
  %this = alloca ptr, align 8
  %2 = alloca %struct.Square, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  store i32 0, ptr %3, align 4
  %4 = load ptr, ptr %this, align 8
  %count.addr = getelementptr inbounds %struct.Counter, ptr %4, i64 0, i32 0
  store i32 0, ptr %count.addr, align 4
  %5 = load ptr, ptr %this, align 8
  %square.addr = getelementptr inbounds %struct.Counter, ptr %5, i64 0, i32 1
  call void @_ZN6Square4ctorEi(ptr noundef nonnull align 8 dereferenceable(16) %2, i32 noundef 3)
  call void @llvm.memcpy.p0.p0.i64(ptr %square.addr, ptr %2, i64 16, i1 false)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_ZN7Counter3incEv(ptr noundef nonnull align 8 dereferenceable(24) %0) #2 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %count.addr = getelementptr inbounds %struct.Counter, ptr %2, i64 0, i32 0
  %3 = load i32, ptr %count.addr, align 4
  %4 = add nsw i32 %3, 1
  store i32 %4, ptr %count.addr, align 4
  %5 = load ptr, ptr %this, align 8
  ret ptr %5
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_ZN7Counter3addEi(ptr noundef nonnull align 8 dereferenceable(24) %0, i32 noundef %1) #2 {
  %this = alloca ptr, align 8
  %value = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %value, align 4
  %3 = load ptr, ptr %this, align 8
  %count.addr = getelementptr inbounds %struct.Counter, ptr %3, i64 0, i32 0
  %4 = load i32, ptr %value, align 4
  %5 = load i32, ptr %count.addr, align 4
  %6 = add nsw i32 %5, %4
  store i32 %6, ptr %count.addr, align 4
  %7 = load ptr, ptr %this, align 8
  ret ptr %7
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN7Counter3getEv(ptr noundef nonnull align 8 dereferenceable(24) %0) #2 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %count.addr = getelementptr inbounds %struct.Counter, ptr %2, i64 0, i32 0
  %3 = load i32, ptr %count.addr, align 4
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_ZN7Counter8getShapeEv(ptr noundef nonnull align 8 dereferenceable(24) %0) #2 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %square.addr = getelementptr inbounds %struct.Counter, ptr %2, i64 0, i32 1
  ret ptr %square.addr
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z8identityP7Counter(ptr noundef align 8 %0) #2 {
  %counter = alloca ptr, align 8
  store ptr %0, ptr %counter, align 8
  %2 = load ptr, ptr %counter, align 8
  ret ptr %2
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #3 {
  %counter = alloca %struct.Counter, align 8
  %1 = alloca ptr, align 8
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca %struct.Counter, align 8
  %7 = alloca %struct.Counter, align 8
  %8 = alloca [2 x %struct.Counter], align 8
  %counters = alloca [2 x %struct.Counter], align 8
  %counterPtr = alloca ptr, align 8
  call void @_ZN7Counter4ctorEv(ptr noundef nonnull align 8 dereferenceable(24) %counter)
  %9 = call noundef ptr @_ZN7Counter3incEv(ptr noundef nonnull align 8 dereferenceable(24) %counter)
  %10 = call noundef ptr @_ZN7Counter3incEv(ptr noundef nonnull align 8 dereferenceable(24) %9)
  %11 = call noundef i32 @_ZN7Counter3getEv(ptr noundef nonnull align 8 dereferenceable(24) %10)
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %11)
  %13 = call noundef ptr @_ZN7Counter3addEi(ptr noundef nonnull align 8 dereferenceable(24) %counter, i32 noundef 5)
  store ptr %13, ptr %1, align 8
  %14 = load ptr, ptr %1, align 8
  %15 = call noundef ptr @_ZN7Counter3incEv(ptr noundef nonnull align 8 dereferenceable(24) %14)
  %16 = call noundef ptr @_ZN7Counter3addEi(ptr noundef nonnull align 8 dereferenceable(24) %15, i32 noundef 10)
  store ptr %16, ptr %2, align 8
  %17 = load ptr, ptr %2, align 8
  %18 = call noundef i32 @_ZN7Counter3getEv(ptr noundef nonnull align 8 dereferenceable(24) %17)
  %19 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %18)
  %20 = call noundef ptr @_Z8identityP7Counter(ptr noundef align 8 %counter)
  store ptr %20, ptr %3, align 8
  %21 = load ptr, ptr %3, align 8
  %22 = call noundef ptr @_ZN7Counter3incEv(ptr noundef nonnull align 8 dereferenceable(24) %21)
  %23 = call noundef i32 @_ZN7Counter3getEv(ptr noundef nonnull align 8 dereferenceable(24) %22)
  %24 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %23)
  %25 = call noundef ptr @_Z8identityP7Counter(ptr noundef align 8 %counter)
  store ptr %25, ptr %4, align 8
  %26 = load ptr, ptr %4, align 8
  %27 = call noundef ptr @_ZN7Counter8getShapeEv(ptr noundef nonnull align 8 dereferenceable(24) %26)
  store ptr %27, ptr %5, align 8
  %28 = load ptr, ptr %5, align 8
  %vtable.addr = load ptr, ptr %28, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  %29 = call noundef i32 %fct(ptr noundef nonnull align 8 dereferenceable(8) %28)
  %30 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %29)
  call void @_ZN7Counter4ctorEv(ptr noundef nonnull align 8 dereferenceable(24) %6)
  call void @_ZN7Counter4ctorEv(ptr noundef nonnull align 8 dereferenceable(24) %7)
  %31 = getelementptr inbounds [2 x %struct.Counter], ptr %8, i64 0
  %32 = load %struct.Counter, ptr %6, align 8
  store %struct.Counter %32, ptr %31, align 8
  %33 = load %struct.Counter, ptr %7, align 8
  %34 = getelementptr inbounds %struct.Counter, ptr %31, i64 1
  store %struct.Counter %33, ptr %34, align 8
  %35 = load [2 x %struct.Counter], ptr %8, align 8
  store [2 x %struct.Counter] %35, ptr %counters, align 8
  %36 = getelementptr inbounds [2 x %struct.Counter], ptr %counters, i64 0, i32 1
  %37 = call noundef ptr @_ZN7Counter3incEv(ptr noundef nonnull align 8 dereferenceable(24) %36)
  %38 = call noundef i32 @_ZN7Counter3getEv(ptr noundef nonnull align 8 dereferenceable(24) %37)
  %39 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %38)
  store ptr %counter, ptr %counterPtr, align 8
  %40 = load ptr, ptr %counterPtr, align 8
  %41 = call noundef ptr @_ZN7Counter3incEv(ptr noundef nonnull align 8 dereferenceable(24) %40)
  %42 = call noundef i32 @_ZN7Counter3getEv(ptr noundef nonnull align 8 dereferenceable(24) %41)
  %43 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %42)
  %44 = load ptr, ptr %counterPtr, align 8
  %45 = call noundef i32 @_ZN7Counter3getEv(ptr noundef nonnull align 8 dereferenceable(24) %44)
  %46 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i32 noundef %45)
  %47 = call noundef ptr @_ZN7Counter3incEv(ptr noundef nonnull align 8 dereferenceable(24) %counter)
  %count.addr = getelementptr inbounds %struct.Counter, ptr %47, i64 0, i32 0
  %48 = load i32, ptr %count.addr, align 4
  %49 = icmp slt i32 %48, 30
  %50 = zext i1 %49 to i32
  %51 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.7, i32 noundef %50)
  %52 = call noundef i32 @_ZN7Counter3getEv(ptr noundef nonnull align 8 dereferenceable(24) %counter)
  %53 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.8, i32 noundef %52)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

attributes #0 = { mustprogress noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { noinline nounwind optnone uwtable }
attributes #3 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #4 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
