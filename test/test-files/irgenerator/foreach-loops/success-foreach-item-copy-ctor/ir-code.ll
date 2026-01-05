; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.ExampleContainedType = type { i1 }
%struct.ExampleTypeIterator = type { %interface.IIterator, %struct.ExampleContainedType, i1 }
%interface.IIterator = type { ptr }
%struct.Pair = type { i64, ptr }
%struct.ExampleIterableType = type { %interface.IIterable }
%interface.IIterable = type { ptr }

$_ZTS19ExampleIterableType = comdat any

$_ZTI19ExampleIterableType = comdat any

$_ZTV19ExampleIterableType = comdat any

$_ZTS19ExampleTypeIteratorI20ExampleContainedTypeE = comdat any

$_ZTI19ExampleTypeIteratorI20ExampleContainedTypeE = comdat any

$_ZTV19ExampleTypeIteratorI20ExampleContainedTypeE = comdat any

@_ZTS19ExampleIterableType = dso_local constant [22 x i8] c"19ExampleIterableType\00", comdat, align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI9IIterableI20ExampleContainedTypeE = external global ptr
@_ZTI19ExampleIterableType = dso_local constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS19ExampleIterableType, ptr @_ZTI9IIterableI20ExampleContainedTypeE }, comdat, align 8
@_ZTV19ExampleIterableType = dso_local unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI19ExampleIterableType, ptr @_ZN19ExampleIterableType11getIteratorEv] }, comdat, align 8
@_ZTS19ExampleTypeIteratorI20ExampleContainedTypeE = dso_local constant [22 x i8] c"19ExampleTypeIterator\00", comdat, align 4
@_ZTI9IIteratorI20ExampleContainedTypeE = external global ptr
@_ZTI19ExampleTypeIteratorI20ExampleContainedTypeE = dso_local constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS19ExampleTypeIteratorI20ExampleContainedTypeE, ptr @_ZTI9IIteratorI20ExampleContainedTypeE }, comdat, align 8
@_ZTV19ExampleTypeIteratorI20ExampleContainedTypeE = dso_local unnamed_addr constant { [6 x ptr] } { [6 x ptr] [ptr null, ptr @_ZTI19ExampleTypeIteratorI20ExampleContainedTypeE, ptr @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE3getEv, ptr @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE6getIdxEv, ptr @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE7isValidEv, ptr @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE4nextEv] }, comdat, align 8
@anon.string.0 = private unnamed_addr constant [61 x i8] c"Assertion failed: Condition 'ct.copied' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'i == 1' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 4

define private void @_ZN20ExampleContainedType4ctorEv(ptr noundef nonnull align 1 dereferenceable(1) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store i1 false, ptr %2, align 1
  ret void
}

define private void @_ZN20ExampleContainedType4ctorERK20ExampleContainedType(ptr noundef nonnull align 1 dereferenceable(1) %0, ptr noundef %1) {
  %this = alloca ptr, align 8
  %_ = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %_, align 8
  %3 = load ptr, ptr %this, align 8
  store i1 false, ptr %3, align 1
  %4 = load ptr, ptr %this, align 8
  %copied.addr = getelementptr inbounds %struct.ExampleContainedType, ptr %4, i64 0, i32 0
  store i1 true, ptr %copied.addr, align 1
  ret void
}

; Function Attrs: norecurse
define void @_ZN19ExampleIterableType4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV19ExampleIterableType, i64 0, i32 0, i32 2), ptr %2, align 8
  ret void
}

; Function Attrs: norecurse
define void @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [6 x ptr] }, ptr @_ZTV19ExampleTypeIteratorI20ExampleContainedTypeE, i64 0, i32 0, i32 2), ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.ExampleTypeIterator, ptr %2, i32 0, i32 1
  call void @_ZN20ExampleContainedType4ctorEv(ptr noundef nonnull align 1 dereferenceable(1) %3)
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
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  %eit = alloca %struct.ExampleIterableType, align 8
  %i = alloca i32, align 4
  %1 = alloca %struct.ExampleTypeIterator, align 8
  %ct = alloca %struct.ExampleContainedType, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN19ExampleIterableType4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %eit)
  store i32 0, ptr %i, align 4
  %2 = call %struct.ExampleTypeIterator @_ZN19ExampleIterableType11getIteratorEv(ptr %eit)
  store %struct.ExampleTypeIterator %2, ptr %1, align 8
  br label %foreach.head.L46

foreach.head.L46:                                 ; preds = %foreach.tail.L46, %0
  %3 = call i1 @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE7isValidEv(ptr %1)
  br i1 %3, label %foreach.body.L46, label %foreach.exit.L46

foreach.body.L46:                                 ; preds = %foreach.head.L46
  %4 = call ptr @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE3getEv(ptr %1)
  call void @_ZN20ExampleContainedType4ctorERK20ExampleContainedType(ptr noundef nonnull align 1 dereferenceable(1) %ct, ptr %4)
  %copied.addr = getelementptr inbounds %struct.ExampleContainedType, ptr %ct, i64 0, i32 0
  %5 = load i1, ptr %copied.addr, align 1
  br i1 %5, label %assert.exit.L47, label %assert.then.L47, !prof !5

assert.then.L47:                                  ; preds = %foreach.body.L46
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L47:                                  ; preds = %foreach.body.L46
  %7 = load i32, ptr %i, align 4
  %8 = add nsw i32 %7, 1
  store i32 %8, ptr %i, align 4
  br label %foreach.tail.L46

foreach.tail.L46:                                 ; preds = %assert.exit.L47
  call void @_ZN19ExampleTypeIteratorI20ExampleContainedTypeE4nextEv(ptr %1)
  br label %foreach.head.L46

foreach.exit.L46:                                 ; preds = %foreach.head.L46
  %9 = load i32, ptr %i, align 4
  %10 = icmp eq i32 %9, 1
  br i1 %10, label %assert.exit.L50, label %assert.then.L50, !prof !5

assert.then.L50:                                  ; preds = %foreach.exit.L46
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L50:                                  ; preds = %foreach.exit.L46
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %13 = load i32, ptr %result, align 4
  ret i32 %13
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

attributes #0 = { norecurse }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { cold noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
