; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Impl = type { %interface.First, %interface.Second, i32 }
%interface.First = type { ptr }
%interface.Second = type { ptr }
%struct.SecondOwner = type { ptr }
%struct.Holder = type { ptr }

@_ZTS5First = private constant [7 x i8] c"5First\00", align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI5First = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5First }, align 8
@_ZTV5First = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI5First, ptr null] }, align 8
@_ZTS6Second = private constant [8 x i8] c"6Second\00", align 4
@_ZTI6Second = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS6Second }, align 8
@_ZTV6Second = private unnamed_addr constant { [4 x ptr] } { [4 x ptr] [ptr null, ptr @_ZTI6Second, ptr null, ptr null] }, align 8
@_ZTS4Impl = private constant [6 x i8] c"4Impl\00", align 4
@_ZTI4Impl = private constant { ptr, ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS4Impl, ptr @_ZTI5First, ptr @_ZTI6Second }, align 8
@_ZTV4Impl = private unnamed_addr constant { [3 x ptr], [4 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI4Impl, ptr @_ZN4Impl3oneEv], [4 x ptr] [ptr null, ptr @_ZTI4Impl, ptr @_ZThn8_N4Impl3twoEv, ptr @_ZThn8_N4Impl3addEi] }, align 8
@printf.str.0 = private unnamed_addr constant [20 x i8] c"Direct: %d, %d, %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [16 x i8] c"Via First*: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [21 x i8] c"Via Second*: %d, %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [21 x i8] c"Via Second&: %d, %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [14 x i8] c"Assigned: %d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [13 x i8] c"Ptr arg: %d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [13 x i8] c"Ref arg: %d\0A\00", align 4
@printf.str.7 = private unnamed_addr constant [18 x i8] c"Returned ptr: %d\0A\00", align 4
@printf.str.8 = private unnamed_addr constant [18 x i8] c"Returned ref: %d\0A\00", align 4
@printf.str.9 = private unnamed_addr constant [11 x i8] c"Field: %d\0A\00", align 4
@printf.str.10 = private unnamed_addr constant [10 x i8] c"Cast: %d\0A\00", align 4
@printf.str.11 = private unnamed_addr constant [9 x i8] c"New: %d\0A\00", align 4
@printf.str.12 = private unnamed_addr constant [19 x i8] c"Placement new: %d\0A\00", align 4
@printf.str.13 = private unnamed_addr constant [17 x i8] c"Pointer ref: %d\0A\00", align 4
@printf.str.14 = private unnamed_addr constant [9 x i8] c"Nil: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Impl4ctorEi(ptr noundef nonnull align 8 dereferenceable(24) %0, i32 noundef %1) #0 {
  %this = alloca ptr, align 8
  %value = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %value, align 4
  %3 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr], [4 x ptr] }, ptr @_ZTV4Impl, i64 0, i32 0, i32 2), ptr %3, align 8
  %4 = getelementptr inbounds nuw %struct.Impl, ptr %3, i32 0, i32 1
  store ptr getelementptr inbounds ({ [3 x ptr], [4 x ptr] }, ptr @_ZTV4Impl, i64 0, i32 1, i32 2), ptr %4, align 8
  %5 = getelementptr inbounds nuw %struct.Impl, ptr %3, i32 0, i32 2
  store i32 0, ptr %5, align 4
  %6 = load ptr, ptr %this, align 8
  %value.addr = getelementptr inbounds %struct.Impl, ptr %6, i64 0, i32 2
  %7 = load i32, ptr %value, align 4
  store i32 %7, ptr %value.addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN4Impl3oneEv(ptr noundef nonnull align 8 dereferenceable(24) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %value.addr = getelementptr inbounds %struct.Impl, ptr %2, i64 0, i32 2
  %3 = load i32, ptr %value.addr, align 4
  %4 = add nsw i32 %3, 1
  ret i32 %4
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN4Impl3twoEv(ptr noundef nonnull align 8 dereferenceable(24) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %value.addr = getelementptr inbounds %struct.Impl, ptr %2, i64 0, i32 2
  %3 = load i32, ptr %value.addr, align 4
  %4 = add nsw i32 %3, 2
  ret i32 %4
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN4Impl3addEi(ptr noundef nonnull align 8 dereferenceable(24) %0, i32 noundef %1) #0 {
  %this = alloca ptr, align 8
  %summand = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %summand, align 4
  %3 = load ptr, ptr %this, align 8
  %value.addr = getelementptr inbounds %struct.Impl, ptr %3, i64 0, i32 2
  %4 = load i32, ptr %summand, align 4
  %5 = load i32, ptr %value.addr, align 4
  %6 = add nsw i32 %5, %4
  ret i32 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN11SecondOwner4ctorEP6Second(ptr noundef nonnull align 8 dereferenceable(8) %0, ptr noundef align 8 %1) #0 {
  %this = alloca ptr, align 8
  %second = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %second, align 8
  %3 = load ptr, ptr %this, align 8
  store ptr null, ptr %3, align 8
  %4 = load ptr, ptr %this, align 8
  %second.addr = getelementptr inbounds %struct.SecondOwner, ptr %4, i64 0, i32 0
  %5 = load ptr, ptr %second, align 8
  store ptr %5, ptr %second.addr, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z9getSecondP4Impl(ptr noundef align 8 %0) #0 {
  %impl = alloca ptr, align 8
  store ptr %0, ptr %impl, align 8
  %2 = load ptr, ptr %impl, align 8
  %3 = getelementptr inbounds nuw %struct.Impl, ptr %2, i32 0, i32 1
  %4 = icmp eq ptr %2, null
  %5 = select i1 %4, ptr null, ptr %3
  ret ptr %5
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z12getSecondRefR4Impl(ptr noundef %0) #0 {
  %impl = alloca ptr, align 8
  store ptr %0, ptr %impl, align 8
  %2 = load ptr, ptr %impl, align 8
  %3 = getelementptr inbounds nuw %struct.Impl, ptr %2, i32 0, i32 1
  ret ptr %3
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z7callPtrP6Second(ptr noundef align 8 %0) #0 {
  %second = alloca ptr, align 8
  store ptr %0, ptr %second, align 8
  %2 = load ptr, ptr %second, align 8
  %vtable.addr = load ptr, ptr %2, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  %3 = call noundef i32 %fct(ptr noundef nonnull align 8 dereferenceable(8) %2)
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z7callRefR6Second(ptr noundef %0) #0 {
  %second = alloca ptr, align 8
  store ptr %0, ptr %second, align 8
  %2 = load ptr, ptr %second, align 8
  %vtable.addr = load ptr, ptr %2, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 1
  %fct = load ptr, ptr %vfct.addr, align 8
  %3 = call noundef i32 %fct(ptr noundef nonnull align 8 dereferenceable(8) %2, i32 noundef 5)
  ret i32 %3
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #1 {
  %impl = alloca %struct.Impl, align 8
  %first = alloca ptr, align 8
  %second = alloca ptr, align 8
  %secondRef = alloca ptr, align 8
  %assigned = alloca ptr, align 8
  %1 = alloca ptr, align 8
  %holder = alloca %struct.Holder, align 8
  %casted = alloca ptr, align 8
  %owner = alloca ptr, align 8
  %placed = alloca ptr, align 8
  %ownerMemory = alloca ptr, align 8
  %implPtr = alloca ptr, align 8
  %firstPtrRef = alloca ptr, align 8
  %nilImpl = alloca ptr, align 8
  %nilSecond = alloca ptr, align 8
  call void @_ZN4Impl4ctorEi(ptr noundef nonnull align 8 dereferenceable(24) %impl, i32 noundef 10)
  %2 = call noundef i32 @_ZN4Impl3oneEv(ptr noundef nonnull align 8 dereferenceable(24) %impl)
  %3 = call noundef i32 @_ZN4Impl3twoEv(ptr noundef nonnull align 8 dereferenceable(24) %impl)
  %4 = call noundef i32 @_ZN4Impl3addEi(ptr noundef nonnull align 8 dereferenceable(24) %impl, i32 noundef 3)
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %2, i32 noundef %3, i32 noundef %4)
  store ptr %impl, ptr %first, align 8
  %6 = load ptr, ptr %first, align 8
  %vtable.addr = load ptr, ptr %6, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  %7 = call noundef i32 %fct(ptr noundef nonnull align 8 dereferenceable(8) %6)
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %7)
  %9 = getelementptr inbounds nuw %struct.Impl, ptr %impl, i32 0, i32 1
  %10 = icmp eq ptr %impl, null
  %11 = select i1 %10, ptr null, ptr %9
  store ptr %11, ptr %second, align 8
  %12 = load ptr, ptr %second, align 8
  %vtable.addr1 = load ptr, ptr %12, align 8
  %vfct.addr2 = getelementptr inbounds ptr, ptr %vtable.addr1, i64 0
  %fct3 = load ptr, ptr %vfct.addr2, align 8
  %13 = call noundef i32 %fct3(ptr noundef nonnull align 8 dereferenceable(8) %12)
  %14 = load ptr, ptr %second, align 8
  %vtable.addr4 = load ptr, ptr %14, align 8
  %vfct.addr5 = getelementptr inbounds ptr, ptr %vtable.addr4, i64 1
  %fct6 = load ptr, ptr %vfct.addr5, align 8
  %15 = call noundef i32 %fct6(ptr noundef nonnull align 8 dereferenceable(8) %14, i32 noundef 3)
  %16 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %13, i32 noundef %15)
  %17 = getelementptr inbounds nuw %struct.Impl, ptr %impl, i32 0, i32 1
  store ptr %17, ptr %secondRef, align 8
  %18 = load ptr, ptr %secondRef, align 8
  %vtable.addr7 = load ptr, ptr %18, align 8
  %vfct.addr8 = getelementptr inbounds ptr, ptr %vtable.addr7, i64 0
  %fct9 = load ptr, ptr %vfct.addr8, align 8
  %19 = call noundef i32 %fct9(ptr noundef nonnull align 8 dereferenceable(8) %18)
  %20 = load ptr, ptr %secondRef, align 8
  %vtable.addr10 = load ptr, ptr %20, align 8
  %vfct.addr11 = getelementptr inbounds ptr, ptr %vtable.addr10, i64 1
  %fct12 = load ptr, ptr %vfct.addr11, align 8
  %21 = call noundef i32 %fct12(ptr noundef nonnull align 8 dereferenceable(8) %20, i32 noundef 4)
  %22 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %19, i32 noundef %21)
  store ptr null, ptr %assigned, align 8
  %23 = getelementptr inbounds nuw %struct.Impl, ptr %impl, i32 0, i32 1
  %24 = icmp eq ptr %impl, null
  %25 = select i1 %24, ptr null, ptr %23
  store ptr %25, ptr %assigned, align 8
  %26 = load ptr, ptr %assigned, align 8
  %vtable.addr13 = load ptr, ptr %26, align 8
  %vfct.addr14 = getelementptr inbounds ptr, ptr %vtable.addr13, i64 0
  %fct15 = load ptr, ptr %vfct.addr14, align 8
  %27 = call noundef i32 %fct15(ptr noundef nonnull align 8 dereferenceable(8) %26)
  %28 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %27)
  %29 = getelementptr inbounds nuw %struct.Impl, ptr %impl, i32 0, i32 1
  %30 = icmp eq ptr %impl, null
  %31 = select i1 %30, ptr null, ptr %29
  %32 = call noundef i32 @_Z7callPtrP6Second(ptr noundef align 8 %31)
  %33 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %32)
  %34 = getelementptr inbounds nuw %struct.Impl, ptr %impl, i32 0, i32 1
  %35 = call noundef i32 @_Z7callRefR6Second(ptr noundef %34)
  %36 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i32 noundef %35)
  %37 = call noundef ptr @_Z9getSecondP4Impl(ptr noundef align 8 %impl)
  store ptr %37, ptr %1, align 8
  %38 = load ptr, ptr %1, align 8
  %vtable.addr16 = load ptr, ptr %38, align 8
  %vfct.addr17 = getelementptr inbounds ptr, ptr %vtable.addr16, i64 0
  %fct18 = load ptr, ptr %vfct.addr17, align 8
  %39 = call noundef i32 %fct18(ptr noundef nonnull align 8 dereferenceable(8) %38)
  %40 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.7, i32 noundef %39)
  %41 = call noundef ptr @_Z12getSecondRefR4Impl(ptr noundef %impl)
  %vtable.addr19 = load ptr, ptr %41, align 8
  %vfct.addr20 = getelementptr inbounds ptr, ptr %vtable.addr19, i64 0
  %fct21 = load ptr, ptr %vfct.addr20, align 8
  %42 = call noundef i32 %fct21(ptr noundef nonnull align 8 dereferenceable(8) %41)
  %43 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.8, i32 noundef %42)
  %44 = getelementptr inbounds nuw %struct.Impl, ptr %impl, i32 0, i32 1
  %45 = icmp eq ptr %impl, null
  %46 = select i1 %45, ptr null, ptr %44
  store ptr %46, ptr %holder, align 8
  %47 = load ptr, ptr %holder, align 8
  %vtable.addr22 = load ptr, ptr %47, align 8
  %vfct.addr23 = getelementptr inbounds ptr, ptr %vtable.addr22, i64 0
  %fct24 = load ptr, ptr %vfct.addr23, align 8
  %48 = call noundef i32 %fct24(ptr noundef nonnull align 8 dereferenceable(8) %47)
  %49 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.9, i32 noundef %48)
  %50 = getelementptr inbounds nuw %struct.Impl, ptr %impl, i32 0, i32 1
  %51 = icmp eq ptr %impl, null
  %52 = select i1 %51, ptr null, ptr %50
  store ptr %52, ptr %casted, align 8
  %53 = load ptr, ptr %casted, align 8
  %vtable.addr25 = load ptr, ptr %53, align 8
  %vfct.addr26 = getelementptr inbounds ptr, ptr %vtable.addr25, i64 0
  %fct27 = load ptr, ptr %vfct.addr26, align 8
  %54 = call noundef i32 %fct27(ptr noundef nonnull align 8 dereferenceable(8) %53)
  %55 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.10, i32 noundef %54)
  %56 = call ptr @_Z12sAllocUnsafem(i64 8)
  %57 = getelementptr inbounds nuw %struct.Impl, ptr %impl, i32 0, i32 1
  %58 = icmp eq ptr %impl, null
  %59 = select i1 %58, ptr null, ptr %57
  call void @_ZN11SecondOwner4ctorEP6Second(ptr noundef nonnull align 8 dereferenceable(8) %56, ptr %59)
  store ptr %56, ptr %owner, align 8
  %60 = load ptr, ptr %owner, align 8
  %61 = load ptr, ptr %60, align 8
  %vtable.addr28 = load ptr, ptr %61, align 8
  %vfct.addr29 = getelementptr inbounds ptr, ptr %vtable.addr28, i64 0
  %fct30 = load ptr, ptr %vfct.addr29, align 8
  %62 = call noundef i32 %fct30(ptr noundef nonnull align 8 dereferenceable(8) %61)
  %63 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.11, i32 noundef %62)
  %64 = load ptr, ptr %owner, align 8
  %second.addr = getelementptr inbounds %struct.SecondOwner, ptr %64, i64 0, i32 0
  store ptr null, ptr %second.addr, align 8
  %65 = load ptr, ptr %owner, align 8
  %66 = getelementptr inbounds nuw %struct.Impl, ptr %impl, i32 0, i32 1
  %67 = icmp eq ptr %impl, null
  %68 = select i1 %67, ptr null, ptr %66
  call void @_ZN11SecondOwner4ctorEP6Second(ptr noundef nonnull align 8 dereferenceable(8) %65, ptr %68)
  store ptr %65, ptr %placed, align 8
  %69 = load ptr, ptr %placed, align 8
  %70 = load ptr, ptr %69, align 8
  %vtable.addr31 = load ptr, ptr %70, align 8
  %vfct.addr32 = getelementptr inbounds ptr, ptr %vtable.addr31, i64 0
  %fct33 = load ptr, ptr %vfct.addr32, align 8
  %71 = call noundef i32 %fct33(ptr noundef nonnull align 8 dereferenceable(8) %70)
  %72 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.12, i32 noundef %71)
  %73 = load ptr, ptr %owner, align 8
  store ptr %73, ptr %ownerMemory, align 8
  call void @_Z8sDeallocRPVh(ptr noundef %ownerMemory)
  call void @_Z8sDeallocRPVh(ptr %ownerMemory)
  store ptr %impl, ptr %implPtr, align 8
  store ptr %implPtr, ptr %firstPtrRef, align 8
  %74 = load ptr, ptr %firstPtrRef, align 8
  %75 = load ptr, ptr %74, align 8
  %vtable.addr34 = load ptr, ptr %75, align 8
  %vfct.addr35 = getelementptr inbounds ptr, ptr %vtable.addr34, i64 0
  %fct36 = load ptr, ptr %vfct.addr35, align 8
  %76 = call noundef i32 %fct36(ptr noundef nonnull align 8 dereferenceable(8) %75)
  %77 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.13, i32 noundef %76)
  store ptr null, ptr %nilImpl, align 8
  %78 = load ptr, ptr %nilImpl, align 8
  %79 = getelementptr inbounds nuw %struct.Impl, ptr %78, i32 0, i32 1
  %80 = icmp eq ptr %78, null
  %81 = select i1 %80, ptr null, ptr %79
  store ptr %81, ptr %nilSecond, align 8
  %82 = load ptr, ptr %nilSecond, align 8
  %83 = icmp eq ptr %82, null
  %84 = zext i1 %83 to i32
  %85 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.14, i32 noundef %84)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

declare ptr @_Z12sAllocUnsafem(i64)

declare void @_Z8sDeallocRPVh(ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZThn8_N4Impl3twoEv(ptr noundef nonnull %0) unnamed_addr #0 {
  %this.adjusted = getelementptr inbounds i8, ptr %0, i64 -8
  %2 = call noundef i32 @_ZN4Impl3twoEv(ptr noundef nonnull align 8 dereferenceable(24) %this.adjusted)
  ret i32 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZThn8_N4Impl3addEi(ptr noundef nonnull %0, i32 noundef %1) unnamed_addr #0 {
  %this.adjusted = getelementptr inbounds i8, ptr %0, i64 -8
  %3 = call noundef i32 @_ZN4Impl3addEi(ptr noundef nonnull align 8 dereferenceable(24) %this.adjusted, i32 noundef %1)
  ret i32 %3
}

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
