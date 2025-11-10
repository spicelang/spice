; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-gnu"

%struct.Vector = type { %interface.IIterable, ptr, i64, i64 }
%interface.IIterable = type { ptr }
%struct.String = type { ptr, i64, i64 }
%struct.VectorIterator = type { %interface.IIterator, ptr, i64 }
%interface.IIterator = type { ptr }

@anon.string.0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 4
@anon.string.1 = private unnamed_addr constant [7 x i8] c"World!\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %stringVec = alloca %struct.Vector, align 8
  %1 = alloca %struct.String, align 8
  %2 = alloca %struct.String, align 8
  %3 = alloca %struct.VectorIterator, align 8
  %str = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN6VectorI6StringE4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %stringVec)
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr @anon.string.0)
  call void @_ZN6VectorI6StringE8pushBackERK6String(ptr noundef nonnull align 8 dereferenceable(32) %stringVec, ptr %1)
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr @anon.string.1)
  call void @_ZN6VectorI6StringE8pushBackERK6String(ptr noundef nonnull align 8 dereferenceable(32) %stringVec, ptr %2)
  %5 = call %struct.VectorIterator @_ZN6VectorI6StringE11getIteratorEv(ptr %stringVec)
  store %struct.VectorIterator %5, ptr %3, align 8
  br label %foreach.head.L8

foreach.head.L8:                                  ; preds = %foreach.tail.L8, %0
  %6 = call i1 @_ZN14VectorIteratorI6StringE7isValidEv(ptr %3)
  br i1 %6, label %foreach.body.L8, label %foreach.exit.L8

foreach.body.L8:                                  ; preds = %foreach.head.L8
  %7 = call ptr @_ZN14VectorIteratorI6StringE3getEv(ptr %3)
  store ptr %7, ptr %4, align 8
  %8 = load ptr, ptr %4, align 8
  %9 = load %struct.String, ptr %8, align 8
  call void @_Z5printI6StringEv6String(%struct.String %9)
  br label %foreach.tail.L8

foreach.tail.L8:                                  ; preds = %foreach.body.L8
  call void @_ZN14VectorIteratorI6StringE4nextEv(ptr %3)
  br label %foreach.head.L8

foreach.exit.L8:                                  ; preds = %foreach.head.L8
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %2)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %1)
  call void @_ZN6VectorI6StringE4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %stringVec)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

declare void @_ZN6VectorI6StringE4ctorEv(ptr noundef nonnull align 8 dereferenceable(32))

declare void @_ZN6String4ctorEPKc(ptr, ptr)

declare void @_ZN6VectorI6StringE8pushBackERK6String(ptr, ptr)

declare %struct.VectorIterator @_ZN6VectorI6StringE11getIteratorEv(ptr)

declare i1 @_ZN14VectorIteratorI6StringE7isValidEv(ptr)

declare ptr @_ZN14VectorIteratorI6StringE3getEv(ptr)

declare void @_Z5printI6StringEv6String(%struct.String)

declare void @_ZN14VectorIteratorI6StringE4nextEv(ptr)

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

declare void @_ZN6VectorI6StringE4dtorEv(ptr noundef nonnull align 8 dereferenceable(32))

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
