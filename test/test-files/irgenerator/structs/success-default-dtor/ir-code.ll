; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Result = type { ptr, %struct.Error }
%struct.Error = type { i32, ptr }
%struct.StructWithHeapFields = type { ptr }

@printf.str.0 = private unnamed_addr constant [16 x i8] c"Is nullptr: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [16 x i8] c"Is nullptr: %d\0A\00", align 4

; Function Attrs: norecurse
define void @_ZN20StructWithHeapFields4dtorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  call void @_Z8sDeallocRPh(ptr %2)
  ret void
}

declare void @_Z8sDeallocRPh(ptr)

define private void @_ZN20StructWithHeapFields4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %this = alloca ptr, align 8
  %res = alloca %struct.Result, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr null, ptr %2, align 8
  %3 = call %struct.Result @_Z6sAllocm(i64 10)
  store %struct.Result %3, ptr %res, align 8
  %4 = load ptr, ptr %this, align 8
  %data.addr = getelementptr inbounds %struct.StructWithHeapFields, ptr %4, i64 0, i32 0
  %5 = call ptr @_ZN6ResultIPhE6unwrapEv(ptr noundef nonnull align 8 dereferenceable(24) %res)
  %6 = load ptr, ptr %5, align 8
  store ptr %6, ptr %data.addr, align 8
  ret void
}

declare %struct.Result @_Z6sAllocm(i64)

declare ptr @_ZN6ResultIPhE6unwrapEv(ptr)

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  %sPtr = alloca ptr, align 8
  %s = alloca %struct.StructWithHeapFields, align 8
  store i32 0, ptr %result, align 4
  store ptr null, ptr %sPtr, align 8
  call void @_ZN20StructWithHeapFields4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %s)
  store ptr %s, ptr %sPtr, align 8
  %data.addr = getelementptr inbounds %struct.StructWithHeapFields, ptr %s, i64 0, i32 0
  %1 = load ptr, ptr %data.addr, align 8
  %2 = icmp eq ptr %1, null
  %3 = zext i1 %2 to i32
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3)
  call void @_ZN20StructWithHeapFields4dtorEv(ptr noundef nonnull align 8 dereferenceable(8) %s)
  %5 = load ptr, ptr %sPtr, align 8
  %data.addr1 = getelementptr inbounds %struct.StructWithHeapFields, ptr %5, i64 0, i32 0
  %6 = load ptr, ptr %data.addr1, align 8
  %7 = icmp eq ptr %6, null
  %8 = zext i1 %7 to i32
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { norecurse }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
