; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.StructWithHeapFields = type { ptr }
%struct.Result = type { ptr, %struct.Error }
%struct.Error = type { i32, ptr }

@printf.str.0 = private unnamed_addr constant [16 x i8] c"Is nullptr: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [16 x i8] c"Is nullptr: %d\0A\00", align 1

; Function Attrs: norecurse
define private void @_ZN20StructWithHeapFields4dtorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %struct.StructWithHeapFields, ptr %2, i32 0, i32 0
  call void @_Z8sDeallocRPh(ptr %3)
  ret void
}

declare void @_Z8sDeallocRPh(ptr)

define private void @_ZN20StructWithHeapFields4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %this = alloca ptr, align 8
  %res = alloca %struct.Result, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %struct.StructWithHeapFields, ptr %2, i32 0, i32 0
  store ptr null, ptr %3, align 8
  %4 = call %struct.Result @_Z6sAllocm(i64 10)
  store %struct.Result %4, ptr %res, align 8
  %5 = load ptr, ptr %this, align 8
  %data_addr = getelementptr inbounds %struct.StructWithHeapFields, ptr %5, i32 0, i32 0
  %6 = call ptr @_ZN6ResultIPhE6unwrapEv(ptr noundef nonnull align 8 dereferenceable(24) %res)
  store ptr %6, ptr %data_addr, align 8
  ret void
}

declare %struct.Result @_Z6sAllocm(i64)

declare ptr @_ZN6ResultIPhE6unwrapEv(ptr)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %sPtr = alloca ptr, align 8
  %s = alloca %struct.StructWithHeapFields, align 8
  store i32 0, ptr %result, align 4
  store ptr null, ptr %sPtr, align 8
  call void @_ZN20StructWithHeapFields4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %s)
  store ptr %s, ptr %sPtr, align 8
  %data_addr = getelementptr inbounds %struct.StructWithHeapFields, ptr %s, i32 0, i32 0
  %1 = load ptr, ptr %data_addr, align 8
  %2 = icmp eq ptr %1, null
  %3 = zext i1 %2 to i32
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  call void @_ZN20StructWithHeapFields4dtorEv(ptr %s)
  %5 = load ptr, ptr %sPtr, align 8
  %data_addr1 = getelementptr inbounds %struct.StructWithHeapFields, ptr %5, i32 0, i32 0
  %6 = load ptr, ptr %data_addr1, align 8
  %7 = icmp eq ptr %6, null
  %8 = zext i1 %7 to i32
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

attributes #0 = { norecurse }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
