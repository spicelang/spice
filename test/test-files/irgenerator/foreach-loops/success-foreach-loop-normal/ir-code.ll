; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Vector = type { %interface.IIterable, ptr, i64, i64 }
%interface.IIterable = type { ptr }

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Item: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %intVector = alloca %struct.Vector, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %iterator.addr = alloca ptr, align 8
  %item = alloca i32, align 4
  call void @_ZN6VectorIiE4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %intVector)
  store i32 1, ptr %1, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %intVector, ptr noundef %1)
  store i32 5, ptr %2, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %intVector, ptr noundef %2)
  store i32 4, ptr %3, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %intVector, ptr noundef %3)
  store i32 0, ptr %4, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %intVector, ptr noundef %4)
  store i32 12, ptr %5, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %intVector, ptr noundef %5)
  store i32 12345, ptr %6, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %intVector, ptr noundef %6)
  store i32 9, ptr %7, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %intVector, ptr noundef %7)
  %8 = call ptr @_ZN6VectorIiE11getIteratorEv(ptr %intVector)
  store ptr %8, ptr %iterator.addr, align 8
  br label %foreach.head.L12

foreach.head.L12:                                 ; preds = %foreach.tail.L12, %0
  %9 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %8)
  br i1 %9, label %foreach.body.L12, label %foreach.exit.L12

foreach.body.L12:                                 ; preds = %foreach.head.L12
  %10 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %8)
  %11 = load i32, ptr %10, align 4
  store i32 %11, ptr %item, align 4
  %12 = load i32, ptr %item, align 4
  %13 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %12)
  br label %foreach.tail.L12

foreach.tail.L12:                                 ; preds = %foreach.body.L12
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %8)
  br label %foreach.head.L12

foreach.exit.L12:                                 ; preds = %foreach.head.L12
  call void @_ZN6VectorIiE4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %intVector)
  call void @_Z8sDeallocRPVh(ptr %iterator.addr)
  ret i32 0
}

declare void @_ZN6VectorIiE4ctorEv(ptr noundef nonnull align 8 dereferenceable(32))

declare void @_ZN6VectorIiE8pushBackERKi(ptr, ptr)

declare ptr @_ZN6VectorIiE11getIteratorEv(ptr)

declare i1 @_ZN14VectorIteratorIiE7isValidEv(ptr)

declare ptr @_ZN14VectorIteratorIiE3getEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

declare void @_ZN14VectorIteratorIiE4nextEv(ptr)

declare void @_ZN6VectorIiE4dtorEv(ptr noundef nonnull align 8 dereferenceable(32))

declare void @_Z8sDeallocRPVh(ptr)

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
