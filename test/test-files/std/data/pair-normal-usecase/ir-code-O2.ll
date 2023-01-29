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
  %1 = alloca %__string_int__Pair__string_int, align 8
  %stringIntPair = alloca %__string_int__Pair__string_int, align 8
  call void @_mp__Pair_string_int__void__ctor__string_int(ptr nonnull %1, ptr nonnull @anon.string.0, i32 1234) #2
  %.fca.0.load = load ptr, ptr %1, align 8
  %.fca.1.gep2 = getelementptr inbounds %__string_int__Pair__string_int, ptr %1, i64 0, i32 1
  %.fca.1.load = load i32, ptr %.fca.1.gep2, align 8
  store ptr %.fca.0.load, ptr %stringIntPair, align 8
  %.fca.1.gep = getelementptr inbounds %__string_int__Pair__string_int, ptr %stringIntPair, i64 0, i32 1
  store i32 %.fca.1.load, ptr %.fca.1.gep, align 8
  %2 = call ptr @_mf__Pair_string_int__string__getFirst(ptr nonnull %stringIntPair) #2
  %3 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, ptr %2)
  %4 = call i32 @_mf__Pair_string_int__int__getSecond(ptr nonnull %stringIntPair) #2
  %5 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %4)
  ret i32 0
}

declare void @_mp__Pair_string_int__void__ctor__string_int(ptr, ptr, i32) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare ptr @_mf__Pair_string_int__string__getFirst(ptr) local_unnamed_addr

declare i32 @_mf__Pair_string_int__int__getSecond(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
