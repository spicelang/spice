; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Point = type { i32, i32 }

@printf.str.0 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 4

define private void @_ZN5Point4ctorEii(ptr noundef nonnull align 4 dereferenceable(8) %0, i32 noundef %1, i32 noundef %2) {
  %this = alloca ptr, align 8
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %x, align 4
  store i32 %2, ptr %y, align 4
  %4 = load ptr, ptr %this, align 8
  store i32 0, ptr %4, align 4
  %5 = getelementptr inbounds nuw %struct.Point, ptr %4, i32 0, i32 1
  store i32 0, ptr %5, align 4
  %6 = load ptr, ptr %this, align 8
  %x.addr = getelementptr inbounds %struct.Point, ptr %6, i64 0, i32 0
  %7 = load i32, ptr %x, align 4
  store i32 %7, ptr %x.addr, align 4
  %8 = load ptr, ptr %this, align 8
  %y.addr = getelementptr inbounds %struct.Point, ptr %8, i64 0, i32 1
  %9 = load i32, ptr %y, align 4
  store i32 %9, ptr %y.addr, align 4
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %storage = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  %1 = call ptr @_Z12sAllocUnsafem(i64 8)
  call void @_ZN5Point4ctorEii(ptr noundef nonnull align 4 dereferenceable(8) %1, i32 0, i32 0)
  store ptr %1, ptr %storage, align 8
  %2 = load ptr, ptr %storage, align 8
  call void @_ZN5Point4ctorEii(ptr noundef nonnull align 4 dereferenceable(8) %2, i32 7, i32 11)
  %3 = load ptr, ptr %storage, align 8
  %x.addr = getelementptr inbounds %struct.Point, ptr %3, i64 0, i32 0
  %4 = load i32, ptr %x.addr, align 4
  %5 = load ptr, ptr %storage, align 8
  %y.addr = getelementptr inbounds %struct.Point, ptr %5, i64 0, i32 1
  %6 = load i32, ptr %y.addr, align 4
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %4, i32 noundef %6)
  call void @_Z8sDeallocRPh(ptr %storage)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

declare ptr @_Z12sAllocUnsafem(i64)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

declare void @_Z8sDeallocRPh(ptr)

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
