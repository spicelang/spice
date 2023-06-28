; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__int__Stack__intptr_long_long = type { ptr, i64, i64 }

@printf.str.0 = private unnamed_addr constant [16 x i8] c"Stack size: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [20 x i8] c"Stack capacity: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [18 x i8] c"Stack item 3: %d\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [18 x i8] c"Stack item 2: %d\0A\00", align 1
@printf.str.4 = private unnamed_addr constant [18 x i8] c"Stack item 1: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %s1 = alloca %__int__Stack__intptr_long_long, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(24) %s1, i8 0, i64 24, i1 false)
  call void @_mp__Stack_int__void__ctor(ptr nonnull %s1) #3
  store i32 123, ptr %1, align 4
  call void @_mp__Stack_int__void__push__intref(ptr nonnull %s1, ptr nonnull %1) #3
  store i32 456, ptr %2, align 4
  call void @_mp__Stack_int__void__push__intref(ptr nonnull %s1, ptr nonnull %2) #3
  store i32 789, ptr %3, align 4
  call void @_mp__Stack_int__void__push__intref(ptr nonnull %s1, ptr nonnull %3) #3
  %4 = call i64 @_mf__Stack_int__long__getSize(ptr nonnull %s1) #3
  %5 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 %4)
  %6 = call i64 @_mf__Stack_int__long__getCapacity(ptr nonnull %s1) #3
  %7 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i64 %6)
  %8 = call ptr @_mf__Stack_int__intref__pop(ptr nonnull %s1) #3
  %9 = load i32, ptr %8, align 4
  %10 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, i32 %9)
  %11 = call ptr @_mf__Stack_int__intref__pop(ptr nonnull %s1) #3
  %12 = load i32, ptr %11, align 4
  %13 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 %12)
  %14 = call ptr @_mf__Stack_int__intref__pop(ptr nonnull %s1) #3
  %15 = load i32, ptr %14, align 4
  %16 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4, i32 %15)
  call void @_mp__Stack_int__void__dtor(ptr nonnull %s1) #3
  ret i32 0
}

declare void @_mp__Stack_int__void__ctor(ptr) local_unnamed_addr

declare void @_mp__Stack_int__void__push__intref(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i64 @_mf__Stack_int__long__getSize(ptr) local_unnamed_addr

declare i64 @_mf__Stack_int__long__getCapacity(ptr) local_unnamed_addr

declare ptr @_mf__Stack_int__intref__pop(ptr) local_unnamed_addr

declare void @_mp__Stack_int__void__dtor(ptr) local_unnamed_addr

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #3 = { nounwind }
