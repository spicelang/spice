; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.ExampleTypeIterator = type { %interface.IIterator, %struct.ExampleContainedType, i1 }
%interface.IIterator = type { ptr }
%struct.ExampleContainedType = type { i32 }
%struct.Pair = type { i64, ptr }
%struct.ExampleIterableType = type { %interface.IIterable }
%interface.IIterable = type { ptr }

$_ZTS19ExampleIterableType = comdat any

$_ZTI19ExampleIterableType = comdat any

$_ZTV19ExampleIterableType = comdat any

$_ZTS19ExampleTypeIteratorI20ExampleContainedTypeE = comdat any

$_ZTI19ExampleTypeIteratorI20ExampleContainedTypeE = comdat any

$_ZTV19ExampleTypeIteratorI20ExampleContainedTypeE = comdat any

@printf.str.0 = private unnamed_addr constant [19 x i8] c"Copy Ctor called!\0A\00", align 4
@_ZTS19ExampleIterableType = dso_local constant [22 x i8] c"19ExampleIterableType\00", comdat, align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI9IIterableI20ExampleContainedTypeE = external global ptr
@_ZTI19ExampleIterableType = dso_local constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS19ExampleIterableType, ptr @_ZTI9IIterableI20ExampleContainedTypeE }, comdat, align 8
@_ZTV19ExampleIterableType = dso_local unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI19ExampleIterableType, ptr @_ZN19ExampleIterableType11getIteratorEv] }, comdat, align 8
@_ZTS19ExampleTypeIteratorI20ExampleContainedTypeE = dso_local constant [22 x i8] c"19ExampleTypeIterator\00", comdat, align 4
@_ZTI9IIteratorI20ExampleContainedTypeE = external global ptr
@_ZTI19ExampleTypeIteratorI20ExampleContainedTypeE = dso_local constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS19ExampleTypeIteratorI20ExampleContainedTypeE, ptr @_ZTI9IIteratorI20ExampleContainedTypeE }, comdat, align 8
@_ZTV19ExampleTypeIteratorI20ExampleContainedTypeE = dso_local unnamed_addr constant { [6 x ptr] } { [6 x ptr] [ptr null, ptr @_ZTI19ExampleTypeIteratorI20ExampleContainedTypeE, ptr @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE3getEv, ptr @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE6getIdxEv, ptr @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE7isValidEv, ptr @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE4nextEv] }, comdat, align 8

define private void @_ZN20ExampleContainedType4ctorEv(ptr noundef nonnull align 4 dereferenceable(4) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store i32 321, ptr %2, align 4
  ret void
}

define private void @_ZN20ExampleContainedType4ctorERK20ExampleContainedType(ptr noundef nonnull align 4 dereferenceable(4) %0, ptr noundef %1) {
  %this = alloca ptr, align 8
  %other = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %other, align 8
  %3 = load ptr, ptr %this, align 8
  store i32 321, ptr %3, align 4
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

; Function Attrs: norecurse
define void @_ZN19ExampleIterableType4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #1 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV19ExampleIterableType, i64 0, i32 0, i32 2), ptr %2, align 8
  ret void
}

; Function Attrs: norecurse
define void @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #1 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [6 x ptr] }, ptr @_ZTV19ExampleTypeIteratorI20ExampleContainedTypeE, i64 0, i32 0, i32 2), ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.ExampleTypeIterator, ptr %2, i32 0, i32 1
  call void @_ZN20ExampleContainedType4ctorEv(ptr noundef nonnull align 4 dereferenceable(4) %3)
  %4 = getelementptr inbounds nuw %struct.ExampleTypeIterator, ptr %2, i32 0, i32 2
  store i1 true, ptr %4, align 1
  ret void
}

define private noundef ptr @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE3getEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %result = alloca ptr, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %item.addr = getelementptr inbounds %struct.ExampleTypeIterator, ptr %2, i64 0, i32 1
  ret ptr %item.addr
}

define private noundef %struct.Pair @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE6getIdxEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %result = alloca %struct.Pair, align 8
  %this = alloca ptr, align 8
  %2 = alloca %struct.Pair, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  %item.addr = getelementptr inbounds %struct.ExampleTypeIterator, ptr %3, i64 0, i32 1
  call void @_ZN4PairImR20ExampleContainedTypeE4ctorEmR20ExampleContainedType(ptr noundef nonnull align 8 dereferenceable(16) %2, i64 noundef 0, ptr noundef %item.addr)
  %4 = load %struct.Pair, ptr %2, align 8
  ret %struct.Pair %4
}

declare void @_ZN4PairImR20ExampleContainedTypeE4ctorEmR20ExampleContainedType(ptr, i64, ptr)

define private noundef zeroext i1 @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE7isValidEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %result = alloca i1, align 1
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %isValid.addr = getelementptr inbounds %struct.ExampleTypeIterator, ptr %2, i64 0, i32 2
  %3 = load i1, ptr %isValid.addr, align 1
  store i1 %3, ptr %result, align 1
  %4 = load ptr, ptr %this, align 8
  %isValid.addr1 = getelementptr inbounds %struct.ExampleTypeIterator, ptr %4, i64 0, i32 2
  store i1 false, ptr %isValid.addr1, align 1
  %5 = load i1, ptr %result, align 1
  ret i1 %5
}

define private void @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE4nextEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  ret void
}

define private noundef %struct.ExampleTypeIterator @_ZN19ExampleIterableType11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %result = alloca %struct.ExampleTypeIterator, align 8
  %this = alloca ptr, align 8
  %2 = alloca %struct.ExampleTypeIterator, align 8
  store ptr %0, ptr %this, align 8
  call void @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %2)
  %3 = load %struct.ExampleTypeIterator, ptr %2, align 8
  ret %struct.ExampleTypeIterator %3
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 {
  %result = alloca i32, align 4
  %eit = alloca %struct.ExampleIterableType, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN19ExampleIterableType4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %eit)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { nofree nounwind }
attributes #1 = { norecurse }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
