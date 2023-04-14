; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.2 = private unnamed_addr constant [9 x i8] c"expected\00", align 1
@anon.string.3 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.4 = private unnamed_addr constant [28 x i8] c"This is not an empty string\00", align 1
@anon.string.5 = private unnamed_addr constant [37 x i8] c"This haystack contains some needles.\00", align 1
@anon.string.6 = private unnamed_addr constant [7 x i8] c"needle\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i16, align 2
  tail call void @_p__void__void__assertTrue__bool(i1 true) #2
  tail call void @_p__void__void__assertFalse__bool(i1 false) #2
  store i32 6, ptr %1, align 4
  store i32 6, ptr %2, align 4
  call void @_p__void__void__assertEqual__intref_intref(ptr nonnull %1, ptr nonnull %2) #2
  store ptr @anon.string.2, ptr %3, align 8
  store ptr @anon.string.2, ptr %4, align 8
  call void @_p__void__void__assertEqual__stringref_stringref(ptr nonnull %3, ptr nonnull %4) #2
  store i32 7, ptr %5, align 4
  store i32 6, ptr %6, align 4
  call void @_p__void__void__assertNotEqual__intref_intref(ptr nonnull %5, ptr nonnull %6) #2
  store i32 0, ptr %7, align 4
  call void @_p__void__void__assertNil__intref(ptr nonnull %7) #2
  store i16 764, ptr %8, align 2
  call void @_p__void__void__assertNotNil__shortref(ptr nonnull %8) #2
  call void @_p__void__void__assertEmpty__string(ptr nonnull @anon.string.3) #2
  call void @_p__void__void__assertNotEmpty__string(ptr nonnull @anon.string.4) #2
  call void @_p__void__void__assertContains__string_string(ptr nonnull @anon.string.5, ptr nonnull @anon.string.6) #2
  %9 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare void @_p__void__void__assertTrue__bool(i1) local_unnamed_addr

declare void @_p__void__void__assertFalse__bool(i1) local_unnamed_addr

declare void @_p__void__void__assertEqual__intref_intref(ptr, ptr) local_unnamed_addr

declare void @_p__void__void__assertEqual__stringref_stringref(ptr, ptr) local_unnamed_addr

declare void @_p__void__void__assertNotEqual__intref_intref(ptr, ptr) local_unnamed_addr

declare void @_p__void__void__assertNil__intref(ptr) local_unnamed_addr

declare void @_p__void__void__assertNotNil__shortref(ptr) local_unnamed_addr

declare void @_p__void__void__assertEmpty__string(ptr) local_unnamed_addr

declare void @_p__void__void__assertNotEmpty__string(ptr) local_unnamed_addr

declare void @_p__void__void__assertContains__string_string(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
