#include <Nebulae/Beta/RenderQueue/DrawItem.h>
#include <Nebulae/Beta/RenderQueue/DrawItemList.h>

#include <cstddef>

#include "gtest/gtest.h"

using namespace Nebulae;

TEST( DrawItemList, Sort_PreservesInsertionOrderForEqualSortKeys )
{
  DrawItemList items;
  items.Add( DrawItem{ 2, 0 } );
  items.Add( DrawItem{ 1, 1 } );
  items.Add( DrawItem{ 1, 2 } );

  items.Sort();

  ASSERT_EQ( std::size_t( 3 ), items.Size() );
  EXPECT_EQ( 1, items[0].sortKey );
  EXPECT_EQ( 1, items[0].submissionOrder );
  EXPECT_EQ( 1, items[1].sortKey );
  EXPECT_EQ( 2, items[1].submissionOrder );
  EXPECT_EQ( 2, items[2].sortKey );
  EXPECT_EQ( 0, items[2].submissionOrder );
}

TEST( DrawItemList, Sort_PreservesSubmissionOrderForSeparatedEqualSortKeys )
{
  DrawItemList items;
  items.Add( DrawItem{ 7, 10 } );
  items.Add( DrawItem{ 3, 11 } );
  items.Add( DrawItem{ 7, 12 } );

  items.Sort();

  ASSERT_EQ( std::size_t( 3 ), items.Size() );
  EXPECT_EQ( 3, items[0].sortKey );
  EXPECT_EQ( 11, items[0].submissionOrder );
  EXPECT_EQ( 7, items[1].sortKey );
  EXPECT_EQ( 10, items[1].submissionOrder );
  EXPECT_EQ( 7, items[2].sortKey );
  EXPECT_EQ( 12, items[2].submissionOrder );
}

TEST( DrawItemList, ClearRetainsAllocatedCapacity )
{
  DrawItemList items;
  items.Add( DrawItem{ 1, 10 } );
  items.Add( DrawItem{ 2, 20 } );
  items.Add( DrawItem{ 3, 30 } );

  const DrawItem* beforeClear = &items[0];

  items.Clear();

  EXPECT_EQ( std::size_t( 0 ), items.Size() );

  items.Add( DrawItem{ 4, 40 } );

  EXPECT_EQ( beforeClear, &items[0] );
}
