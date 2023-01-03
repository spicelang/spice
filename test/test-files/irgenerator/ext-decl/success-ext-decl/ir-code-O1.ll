; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

; Function Attrs: inaccessiblememonly mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0)
declare noalias noundef ptr @malloc(i64 noundef) local_unnamed_addr #0

; Function Attrs: inaccessiblemem_or_argmemonly mustprogress nounwind willreturn allockind("free")
declare void @free(ptr allocptr nocapture noundef) local_unnamed_addr #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #2 {
  %1 = tail call dereferenceable_or_null(1) ptr @malloc(i64 1)
  %2 = ptrtoint ptr %1 to i64
  %address.0.insert.mask = and i64 %2, -256
  %address.0.insert.insert = or i64 %address.0.insert.mask, 12
  %3 = inttoptr i64 %address.0.insert.insert to ptr
  tail call void @free(ptr %3)
  ret i32 0
}

attributes #0 = { inaccessiblememonly mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) "alloc-family"="malloc" }
attributes #1 = { inaccessiblemem_or_argmemonly mustprogress nounwind willreturn allockind("free") "alloc-family"="malloc" }
attributes #2 = { noinline nounwind optnone uwtable }
