; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__string_int__Pair__string_int = type { ptr, i32 }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@printf.str.0 = private unnamed_addr constant [11 x i8] c"First: %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Second: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %stringIntPair = alloca %__string_int__Pair__string_int, align 8
  %1 = alloca ptr, align 8
  %2 = alloca i32, align 4
  store ptr @anon.string.0, ptr %1, align 8
  store i32 1234, ptr %2, align 4
  call void @_mp__Pair_string_int__void__ctor__stringref_intref(ptr nonnull %stringIntPair, ptr nonnull %1, ptr nonnull %2) #2
  %3 = call ptr @_mf__Pair_string_int__stringref__getFirst(ptr nonnull %stringIntPair) #2
  %4 = load ptr, ptr %3, align 8
  %5 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, ptr %4)
  %6 = call ptr @_mf__Pair_string_int__intref__getSecond(ptr nonnull %stringIntPair) #2
  %7 = load i32, ptr %6, align 4
  %8 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %7)
  ret i32 0
}

declare void @_mp__Pair_string_int__void__ctor__stringref_intref(ptr, ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare ptr @_mf__Pair_string_int__stringref__getFirst(ptr) local_unnamed_addr

declare ptr @_mf__Pair_string_int__intref__getSecond(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
