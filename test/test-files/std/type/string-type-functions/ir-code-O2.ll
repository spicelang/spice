; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [5 x i8] c"5.67\00", align 1
@anon.string.1 = private unnamed_addr constant [6 x i8] c"-6546\00", align 1
@anon.string.2 = private unnamed_addr constant [3 x i8] c"56\00", align 1
@anon.string.3 = private unnamed_addr constant [4 x i8] c"-12\00", align 1
@anon.string.4 = private unnamed_addr constant [3 x i8] c"12\00", align 1
@anon.string.5 = private unnamed_addr constant [2 x i8] c"i\00", align 1
@anon.string.6 = private unnamed_addr constant [5 x i8] c"true\00", align 1
@anon.string.8 = private unnamed_addr constant [6 x i8] c"false\00", align 1
@printf.str.0 = private unnamed_addr constant [25 x i8] c"All assertions succeeded\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
assert.exit.L35:
  %0 = tail call double @_Z8toDoublePc(ptr nonnull @anon.string.0) #2
  %1 = tail call i32 @_Z5toIntPc(ptr nonnull @anon.string.1) #2
  %2 = tail call i64 @_Z6toLongPc(ptr nonnull @anon.string.2) #2
  %3 = tail call i16 @_Z7toShortPc(ptr nonnull @anon.string.3) #2
  %4 = tail call i8 @_Z6toBytePc(ptr nonnull @anon.string.4) #2
  %5 = tail call i8 @_Z6toCharPc(ptr nonnull @anon.string.5) #2
  %6 = tail call i1 @_Z6toBoolPc(ptr nonnull @anon.string.6) #2
  %7 = tail call i1 @_Z6toBoolPc(ptr nonnull @anon.string.8) #2
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare double @_Z8toDoublePc(ptr) local_unnamed_addr

declare i32 @_Z5toIntPc(ptr) local_unnamed_addr

declare i64 @_Z6toLongPc(ptr) local_unnamed_addr

declare i16 @_Z7toShortPc(ptr) local_unnamed_addr

declare i8 @_Z6toBytePc(ptr) local_unnamed_addr

declare i8 @_Z6toCharPc(ptr) local_unnamed_addr

declare i1 @_Z6toBoolPc(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
