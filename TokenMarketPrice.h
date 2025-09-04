#ifndef TOKEN_MARKET_PRICE_H
#define TOKEN_MARKET_PRICE_H

const float TOKEN_MIN_PRICE = 4.50;
const float TOKEN_MAX_PRICE = 7.50;

static float TOKEN_MARKET_PRICE = 6.00;

/**
 * Update TOKEN_MARKET_PRICE to a new random value between the TOKEN_MIN_PRICE & TOKEN_MAX_PRICE
 * This is used to make the value of a token fluctuate like a stock.
 */
void refresh_token_market_price()
{
  TOKEN_MARKET_PRICE = random((long)(TOKEN_MIN_PRICE * 100), (long)(TOKEN_MAX_PRICE * 100 + 1)) / 100.0;
}

#endif