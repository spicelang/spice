; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Hello from thread 1\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [21 x i8] c"Hello from thread 2\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @pthread_create(ptr noundef, ptr noundef, { ptr, ptr } noundef, ptr noundef)

declare i32 @pthread_join(i64 noundef, ptr noundef)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %tid1 = alloca i64, align 8
  %tid2 = alloca i64, align 8
  %i = alloca i32, align 4
  %d = alloca double, align 8
  %captures = alloca { ptr }, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %captures1 = alloca { ptr, ptr }, align 8
  %fat.ptr2 = alloca { ptr, ptr }, align 8
  store i32 0, ptr %result, align 4
  store i64 0, ptr %tid1, align 8
  store i64 0, ptr %tid2, align 8
  store i32 123, ptr %i, align 4
  store double 1.234560e+02, ptr %d, align 8
  %1 = getelementptr inbounds { ptr }, ptr %captures, i32 0, i32 0
  store ptr %i, ptr %1, align 8
  %2 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 0
  store ptr @_Z15lambda.L11C39.0v, ptr %2, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %3, align 8
  %4 = load { ptr, ptr }, ptr %fat.ptr, align 8
  %5 = call i32 @pthread_create(ptr %tid1, ptr null, { ptr, ptr } %4, ptr null)
  %6 = getelementptr inbounds { ptr, ptr }, ptr %captures1, i32 0, i32 0
  store ptr %d, ptr %6, align 8
  %7 = getelementptr inbounds { ptr, ptr }, ptr %captures1, i32 0, i32 1
  store ptr %i, ptr %7, align 8
  %8 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr2, i32 0, i32 0
  store ptr @_Z15lambda.L15C39.0v, ptr %8, align 8
  %9 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr2, i32 0, i32 1
  store ptr %captures1, ptr %9, align 8
  %10 = load { ptr, ptr }, ptr %fat.ptr2, align 8
  %11 = call i32 @pthread_create(ptr %tid2, ptr null, { ptr, ptr } %10, ptr null)
  %12 = load i64, ptr %tid1, align 8
  %13 = call i32 @pthread_join(i64 %12, ptr null)
  %14 = load i64, ptr %tid2, align 8
  %15 = call i32 @pthread_join(i64 %14, ptr null)
  %16 = load i32, ptr %i, align 4
  %17 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %16)
  %18 = load i32, ptr %result, align 4
  ret i32 %18
}

define private void @_Z15lambda.L11C39.0v(ptr noundef nonnull dereferenceable(8) %0) {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  %2 = load ptr, ptr %captures, align 8
  %i = getelementptr inbounds { ptr }, ptr %2, i32 0, i32 0
  %3 = load ptr, ptr %i, align 8
  %4 = load i32, ptr %3, align 4
  %5 = add nsw i32 %4, 1
  store volatile i32 %5, ptr %3, align 4
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

define private void @_Z15lambda.L15C39.0v(ptr noundef nonnull dereferenceable(8) %0) {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  %2 = load ptr, ptr %captures, align 8
  %d = getelementptr inbounds { ptr, ptr }, ptr %2, i32 0, i32 0
  %i = getelementptr inbounds { ptr, ptr }, ptr %2, i32 0, i32 1
  %3 = load ptr, ptr %i, align 8
  %4 = load i32, ptr %3, align 4
  %5 = add nsw i32 %4, 1
  store volatile i32 %5, ptr %3, align 4
  %6 = load ptr, ptr %d, align 8
  %7 = load double, ptr %6, align 8
  %8 = fadd double %7, 1.230000e+00
  store volatile double %8, ptr %6, align 8
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
