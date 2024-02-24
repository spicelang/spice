; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [23 x i8] c"Hello from inside: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [23 x i8] c"Hello from inside: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [24 x i8] c"Hello from outside: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %captures = alloca { i32, ptr }, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %lambda = alloca { ptr, ptr }, align 8
  store i32 0, ptr %result, align 4
  store i32 123, ptr %i, align 4
  store i32 321, ptr %j, align 4
  %1 = load i32, ptr %j, align 4
  store i32 %1, ptr %captures, align 4
  %2 = getelementptr inbounds { i32, ptr }, ptr %captures, i32 0, i32 1
  store ptr %i, ptr %2, align 8
  store ptr @_Z14lambda.L4C18.0v, ptr %fat.ptr, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %3, align 8
  %4 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %4, ptr %lambda, align 8
  %5 = getelementptr inbounds { ptr, ptr }, ptr %lambda, i32 0, i32 1
  %captures1 = load ptr, ptr %5, align 8
  %fct = load ptr, ptr %lambda, align 8
  call void %fct(ptr %captures1)
  %6 = load i32, ptr %i, align 4
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %6)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

define private void @_Z14lambda.L4C18.0v(ptr noundef nonnull dereferenceable(8) %0) {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  %2 = load ptr, ptr %captures, align 8
  %i.addr = getelementptr inbounds { i32, ptr }, ptr %2, i32 0, i32 1
  %3 = load ptr, ptr %i.addr, align 8
  %4 = load i32, ptr %3, align 4
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %4)
  %6 = load ptr, ptr %i.addr, align 8
  %7 = load i32, ptr %6, align 4
  %8 = add nsw i32 %7, 1
  store i32 %8, ptr %6, align 4
  %9 = load i32, ptr %2, align 4
  %10 = load ptr, ptr %i.addr, align 8
  %11 = load i32, ptr %10, align 4
  %12 = add nsw i32 %11, %9
  store i32 %12, ptr %10, align 4
  %13 = load ptr, ptr %i.addr, align 8
  %14 = load i32, ptr %13, align 4
  %15 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %14)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
