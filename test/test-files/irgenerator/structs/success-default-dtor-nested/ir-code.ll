; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Inner = type { ptr, ptr }
%struct.Middle = type { %struct.Inner }
%struct.Outer = type { %struct.Middle }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@printf.str.0 = private unnamed_addr constant [19 x i8] c"Inner dtor called\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1

declare ptr @malloc(i64)

declare void @free(ptr)

define private void @_ZN5Inner4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store %struct.Inner { ptr @anon.string.0, ptr null }, ptr %0, align 8
  %2 = load ptr, ptr %this, align 8
  %data_addr = getelementptr inbounds %struct.Inner, ptr %2, i32 0, i32 1
  %3 = call ptr @malloc(i64 10)
  store ptr %3, ptr %data_addr, align 8
  ret void
}

define private void @_ZN5Inner4dtorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %data_addr = getelementptr inbounds %struct.Inner, ptr %2, i32 0, i32 1
  %3 = load ptr, ptr %data_addr, align 8
  call void @free(ptr %3)
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

; Function Attrs: norecurse
define private void @_ZN6Middle4dtorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #1 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %struct.Middle, ptr %2, i32 0, i32 0
  call void @_ZN5Inner4dtorEv(ptr %3)
  ret void
}

; Function Attrs: norecurse
define private void @_ZN5Outer4dtorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #1 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %struct.Outer, ptr %2, i32 0, i32 0
  call void @_ZN6Middle4dtorEv(ptr %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #2 {
  %result = alloca i32, align 4
  %outer = alloca %struct.Outer, align 8
  store i32 0, ptr %result, align 4
  store %struct.Outer { %struct.Middle { %struct.Inner { ptr @anon.string.1, ptr null } } }, ptr %outer, align 8
  call void @_ZN5Outer4dtorEv(ptr %outer)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { nofree nounwind }
attributes #1 = { norecurse }
attributes #2 = { noinline nounwind optnone uwtable }
