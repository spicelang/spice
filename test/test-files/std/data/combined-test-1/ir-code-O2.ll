; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%"__std/data/pair::Pair<int,string>__Vector__std/data/pair::Pair<int,string>ptr_long_long_int" = type { ptr, i64, i64, i32 }
%__int_string__Pair__int_string = type { i32, ptr }

@anon.string.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.1 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@printf.str.0 = private unnamed_addr constant [11 x i8] c"Hello %s!\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %pairVector = alloca %"__std/data/pair::Pair<int,string>__Vector__std/data/pair::Pair<int,string>ptr_long_long_int", align 8
  %1 = alloca %__int_string__Pair__int_string, align 8
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca %__int_string__Pair__int_string, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  call void @"_mp__Vector_std/data/pair::Pair<int,string>__void__ctor"(ptr nonnull %pairVector) #2
  store i32 0, ptr %2, align 4
  store ptr @anon.string.0, ptr %3, align 8
  call void @_mp__Pair_int_string__void__ctor__intref_stringref(ptr nonnull %1, ptr nonnull %2, ptr nonnull %3) #2
  call void @"_mp__Vector_std/data/pair::Pair<int,string>__void__pushBack__std/data/pair::Pair<int,string>ref"(ptr nonnull %pairVector, ptr nonnull %1) #2
  store i32 1, ptr %5, align 4
  store ptr @anon.string.1, ptr %6, align 8
  call void @_mp__Pair_int_string__void__ctor__intref_stringref(ptr nonnull %4, ptr nonnull %5, ptr nonnull %6) #2
  call void @"_mp__Vector_std/data/pair::Pair<int,string>__void__pushBack__std/data/pair::Pair<int,string>ref"(ptr nonnull %pairVector, ptr nonnull %4) #2
  %p1 = call ptr @"_mf__Vector_std/data/pair::Pair<int,string>__std/data/pair::Pair<int,string>ref__get__int"(ptr nonnull %pairVector, i32 1) #2
  %7 = call ptr @_mf__Pair_int_string__stringref__getSecond(ptr %p1) #2
  %8 = load ptr, ptr %7, align 8
  %9 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, ptr %8)
  ret i32 0
}

declare void @"_mp__Vector_std/data/pair::Pair<int,string>__void__ctor"(ptr) local_unnamed_addr

declare void @_mp__Pair_int_string__void__ctor__intref_stringref(ptr, ptr, ptr) local_unnamed_addr

declare void @"_mp__Vector_std/data/pair::Pair<int,string>__void__pushBack__std/data/pair::Pair<int,string>ref"(ptr, ptr) local_unnamed_addr

declare ptr @"_mf__Vector_std/data/pair::Pair<int,string>__std/data/pair::Pair<int,string>ref__get__int"(ptr, i32) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare ptr @_mf__Pair_int_string__stringref__getSecond(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
