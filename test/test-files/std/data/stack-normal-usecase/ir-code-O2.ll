; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__int__Stack__intptr_long_long_int = type { ptr, i64, i64, i32 }

@printf.str.0 = private unnamed_addr constant [16 x i8] c"Stack size: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [20 x i8] c"Stack capacity: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [18 x i8] c"Stack item 3: %d\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [18 x i8] c"Stack item 2: %d\0A\00", align 1
@printf.str.4 = private unnamed_addr constant [18 x i8] c"Stack item 1: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %s1 = alloca %__int__Stack__intptr_long_long_int, align 8
  call void @_mp__Stack_int__void__ctor(ptr nonnull %s1) #2
  call void @_mp__Stack_int__void__push__int(ptr nonnull %s1, i32 123) #2
  call void @_mp__Stack_int__void__push__int(ptr nonnull %s1, i32 456) #2
  call void @_mp__Stack_int__void__push__int(ptr nonnull %s1, i32 789) #2
  %1 = call i64 @_mf__Stack_int__long__getSize(ptr nonnull %s1) #2
  %2 = call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i64 %1)
  %3 = call i64 @_mf__Stack_int__long__getCapacity(ptr nonnull %s1) #2
  %4 = call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i64 %3)
  %5 = call i32 @_mf__Stack_int__int__pop(ptr nonnull %s1) #2
  %6 = call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.2, i32 %5)
  %7 = call i32 @_mf__Stack_int__int__pop(ptr nonnull %s1) #2
  %8 = call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.3, i32 %7)
  %9 = call i32 @_mf__Stack_int__int__pop(ptr nonnull %s1) #2
  %10 = call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.4, i32 %9)
  ret i32 0
}

declare void @_mp__Stack_int__void__ctor(ptr) local_unnamed_addr

declare void @_mp__Stack_int__void__push__int(ptr, i32) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i64 @_mf__Stack_int__long__getSize(ptr) local_unnamed_addr

declare i64 @_mf__Stack_int__long__getCapacity(ptr) local_unnamed_addr

declare i32 @_mf__Stack_int__int__pop(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
