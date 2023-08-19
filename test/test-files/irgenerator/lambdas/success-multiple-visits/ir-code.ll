; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%anon.captures.0 = type { i32, i32 }
%anon.captures.0.0 = type { i32, i32 }

@printf.str.0 = private unnamed_addr constant [8 x i8] c"%d, %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [8 x i8] c"%d, %d\0A\00", align 1

define private void @_Z4testv() {
  %t = alloca i32, align 4
  %x = alloca i32, align 4
  %captures = alloca %anon.captures.0, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %l = alloca ptr, align 8
  store i32 123, ptr %t, align 4
  store i32 456, ptr %x, align 4
  %1 = load i32, ptr %t, align 4
  %2 = getelementptr inbounds %anon.captures.0, ptr %captures, i32 0, i32 0
  store i32 %1, ptr %2, align 4
  %3 = load i32, ptr %x, align 4
  %4 = getelementptr inbounds %anon.captures.0, ptr %captures, i32 0, i32 1
  store i32 %3, ptr %4, align 4
  %5 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 0
  store ptr @_Z14lambda.L3C13.0v, ptr %5, align 8
  %6 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %6, align 8
  store ptr %fat.ptr, ptr %l, align 8
  %7 = load ptr, ptr %l, align 8
  %8 = getelementptr inbounds { ptr, ptr }, ptr %7, i32 0, i32 0
  %9 = getelementptr inbounds { ptr, ptr }, ptr %7, i32 0, i32 1
  %10 = load ptr, ptr %9, align 8
  %11 = load ptr, ptr %8, align 8
  call void %11(ptr %10)
  ret void
}

define private void @_Z14lambda.L3C13.0v(ptr noundef nonnull dereferenceable(8) %0) {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  %2 = load ptr, ptr %captures, align 8
  %3 = getelementptr inbounds %anon.captures.0, ptr %2, i32 0, i32 0
  %4 = getelementptr inbounds %anon.captures.0, ptr %2, i32 0, i32 1
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %5, i32 %6)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

define private void @_Z4testi(i32 %0) {
  %t = alloca i32, align 4
  %x = alloca i32, align 4
  %captures = alloca %anon.captures.0.0, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %l = alloca ptr, align 8
  store i32 %0, ptr %t, align 4
  store i32 456, ptr %x, align 4
  %2 = load i32, ptr %t, align 4
  %3 = getelementptr inbounds %anon.captures.0.0, ptr %captures, i32 0, i32 0
  store i32 %2, ptr %3, align 4
  %4 = load i32, ptr %x, align 4
  %5 = getelementptr inbounds %anon.captures.0.0, ptr %captures, i32 0, i32 1
  store i32 %4, ptr %5, align 4
  %6 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 0
  store ptr @_Z14lambda.L3C13.1v, ptr %6, align 8
  %7 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %7, align 8
  store ptr %fat.ptr, ptr %l, align 8
  %8 = load ptr, ptr %l, align 8
  %9 = getelementptr inbounds { ptr, ptr }, ptr %8, i32 0, i32 0
  %10 = getelementptr inbounds { ptr, ptr }, ptr %8, i32 0, i32 1
  %11 = load ptr, ptr %10, align 8
  %12 = load ptr, ptr %9, align 8
  call void %12(ptr %11)
  ret void
}

define private void @_Z14lambda.L3C13.1v(ptr noundef nonnull dereferenceable(8) %0) {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  %2 = load ptr, ptr %captures, align 8
  %3 = getelementptr inbounds %anon.captures.0.0, ptr %2, i32 0, i32 0
  %4 = getelementptr inbounds %anon.captures.0.0, ptr %2, i32 0, i32 1
  %5 = load i32, ptr %4, align 4
  %6 = load i32, ptr %3, align 4
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %5, i32 %6)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  call void @_Z4testv()
  call void @_Z4testi(i32 4321)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
